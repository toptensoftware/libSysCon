#include "libSysCon.h"
#include "../libFatFS/ff.h"
#include <string.h>
#include <stdio.h>

#define MAX_FILENAME_BUFFER	1024		// Max size of all filename strings
#define MAX_FILENAMES		128			// Max number of file

typedef struct tagCHOOSEFILECONTEXT
{
	const char* pattern;
	char* pBufferBase;
	char* pBufferPos;
	const char** ppFiles;
	uint8_t nFiles;
	uint8_t nPathParts;
	const char* pszNullOption;
	const char* pszDirectory;
	const char* pszSelection;
	char szTemp[48];
} CHOOSEFILECONTEXT;

#define EK_PATH_PART	((char)2)
#define EK_DIRECTORY	((char)3)
#define EK_FILE			((char)4)

static bool add_entry(CHOOSEFILECONTEXT* pctx, const char* filename, char entryKind)
{
	// Room in the filename buffer
	int len = strlen(filename) + 1;	// +1 byte for entry kind
	if (pctx->pBufferPos + len + 1 > pctx->pBufferBase + MAX_FILENAME_BUFFER)
		return false;

	// Room in the filenam pointer buffer
	if (pctx->nFiles == MAX_FILENAMES)
		return false;

	// Copy to buffer
	pctx->pBufferPos[0] = entryKind;
	strcpy(pctx->pBufferPos+1, filename);

	// Update file count and buffer position
	pctx->ppFiles[pctx->nFiles++] = pctx->pBufferPos;
	pctx->pBufferPos += len + 1;		// +1 for null 

	return true;
}


static void sort_files(const char** ppFiles, size_t count)
{
  size_t i;
  size_t j;
  const char** pi = ppFiles;
  const char** pj;
  const char* tmp;

  for (i=0; i<count-1; i++, pi++)
  {
    pj = pi + 1;
    for (j=i+1; j<count; j++, pj++)
    {
      if (stricmp(*pi, *pj) > 0)
      {
      	tmp = *pi;
      	*pi = *pj;
      	*pj = tmp;
      }
    }
  }
}

int indexOf(const char** ppFiles, size_t count, const char* psz)
{
	size_t i;

	if (!psz)
		return -1;

	for (i=0; i<count; i++)
	{
		if (strcmp(ppFiles[i] + 1, psz)==0)
			return i;
	}

	return -1;
}

// From libFatFS
int pattern_matching (	/* 0:not matched, 1:matched */
	const TCHAR* pat,	/* Matching pattern */
	const TCHAR* nam,	/* String to be tested */
	int skip,			/* Number of pre-skip chars (number of ?s) */
	int inf				/* Infinite search (* specified) */
);


int choose_file_getItemCount(struct tagLISTBOX* pListBox)
{
	CHOOSEFILECONTEXT* pctx = pListBox->window.user;

	int count =  pctx->nFiles;

	if (pctx->pszNullOption)
		count++;	

	return count;
}

const char* choose_file_getItemText(struct tagLISTBOX* pListBox, int item)
{
	CHOOSEFILECONTEXT* pctx = pListBox->window.user;

	if (pctx->pszNullOption)
	{
		if (item == 0)
			return pctx->pszNullOption;
		item--;
	}

	// Path part?
	if (pctx->ppFiles[item][0] == EK_PATH_PART)
	{
		if (item > 0)
			memset(pctx->szTemp, ' ', item * 2);
		strcpy(pctx->szTemp + item, pctx->ppFiles[item] + 1);
		return pctx->szTemp;
	}

	// Indent directory/file name
	memset(pctx->szTemp, ' ', pctx->nPathParts);
	strcpy(pctx->szTemp + pctx->nPathParts, pctx->ppFiles[item] + 1);

	// Directory
	if (pctx->ppFiles[item][0] == EK_DIRECTORY)
	{
		strcat(pctx->szTemp, "/");
	}

	return pctx->szTemp;
}

void add_dir_part(CHOOSEFILECONTEXT* pctx, const char* pszPart, int len)
{
	memcpy(pctx->szTemp, pszPart, len);
	pctx->szTemp[len] = '/';
	pctx->szTemp[len+1] = '\0';
	add_entry(pctx, pctx->szTemp, EK_PATH_PART);
}

int choose_file_load_content(CHOOSEFILECONTEXT* pctx)
{
	// Reset state
	pctx->nFiles = 0;
	pctx->pBufferPos = pctx->pBufferBase;

	// Create path element entries
	const char* p = pctx->pszDirectory;
	const char* pszPart = p;
	while (*p)
	{
		if (*p == '\\' || *p == '/')
		{
			add_dir_part(pctx, pszPart, p-pszPart);
			pszPart = p + 1;
		}
		p++;
	}
	if (pctx->nFiles == 0 || p > pszPart)
		add_dir_part(pctx, pszPart, p-pszPart);

	// Store the number of path parts
	pctx->nPathParts = pctx->nFiles;

	// Now enumerate files and folders in the directory
	DIR dr;
	FRESULT r = f_opendir(&dr, pctx->pszDirectory);
	if (r == FR_OK)
	{
		// Get directory item
		FILINFO filinfo;
		r = f_readdir(&dr, &filinfo);

		while (r == FR_OK && filinfo.fname[0])
		{
			if (filinfo.fattrib & AM_DIR)
			{
				// Directory
				add_entry(pctx, filinfo.fname, EK_DIRECTORY);
			}
			else if (pattern_matching("*.cas", filinfo.fname, 0, 0))
			{
				// Matching file
				add_entry(pctx, filinfo.fname, EK_FILE);
			}

			r = f_readdir(&dr, &filinfo);		/* Get a directory item */
		}

	    f_closedir(&dr);
	}
	
	// Sort 
	sort_files(pctx->ppFiles, pctx->nFiles);

	// Work out index
	int sel = -1;
	if (pctx->pszSelection == NULL)
		sel =  pctx->nPathParts - 1;
	else
		sel = indexOf(pctx->ppFiles, pctx->nFiles, pctx->pszSelection);

	// Adjust for leading items
	if (pctx->pszNullOption)
		sel++;	

	return sel;
}

const char* get_selected_path(CHOOSEFILECONTEXT* pctx, int sel, bool* pbIsDir)
{
	// Nothing selected?
	if (sel < 0)
		return NULL;

	// Null item selected?
	if (pctx->pszNullOption)
	{
		if (sel == 0)
			return stralloc("");
		sel--;
	}

	// Work out length of path elements
	uint8_t nPathElements = sel < pctx->nPathParts ? sel + 1 : pctx->nPathParts;
	uint8_t len = 1;
	int i;
	for (i=0; i<nPathElements; i++)
	{
		len += strlen(pctx->ppFiles[i]+1);		// +1 = skip entry type
	}

	// Also allow room for the file name itself
	if (sel >= pctx->nPathParts)
	{
		len += strlen(pctx->ppFiles[sel]+1);	// +1 = skip entry type
		*pbIsDir = pctx->ppFiles[sel][0] != EK_FILE;
	}
	else
	{
		*pbIsDir = true;	
	}

	// Build new path string
	char* psz = malloc(len);
	psz[0] = '\0';
	for (i=0; i<nPathElements; i++)
	{
		strcat(psz, pctx->ppFiles[i]+1);
	}

	// Append the select file in the current directory
	if (sel >= pctx->nPathParts)
		strcat(psz, pctx->ppFiles[sel] + 1);

	// Remove trailing slash
	len = strlen(psz);
	if (len > 1 && psz[len-1] == '/')
		psz[len-1] = '\0';

	return psz;
}

size_t choosefile_wndproc(WINDOW* pWindow, MSG* pMsg)
{
	if (pMsg->message == MESSAGE_KEYDOWN)
	{
		switch (pMsg->param1)
		{
			case KEY_ENTER:
			{
				LISTBOX* pListBox = (LISTBOX*)pWindow;
				CHOOSEFILECONTEXT* pctx = pListBox->window.user;

				// Get the selected item
				bool bIsDir = false;
				const char* pszPath = get_selected_path(pctx, pListBox->selectedItem, &bIsDir);

				// Is it a path part?
				if (bIsDir)
				{
					// Remove old selection bar
					listbox_update(pListBox, -1, pListBox->scrollPos);

					// Load new content
					pctx->pszDirectory = pszPath;
					pctx->pszSelection = NULL;
					pListBox->scrollPos = 0;
					pListBox->selectedItem = choose_file_load_content(pctx);
					window_invalidate(pWindow);

					free(pctx->pszDirectory);
					return 0;
				}
				else
				{
					// Store selected file
					pctx->pszSelection = pszPath;
				}
				break;
			}
		}
	}

	return listbox_wndproc(pWindow, pMsg);
}

const char* choose_file_ex(
	const char* pattern, 
	const char* selectedFile, 
	const char* pszNullOption, 
	const char* pszTitle,
	size_t (*wndProc)(WINDOW* pWindow, MSG* pMsg)
	)
{
	int err;	
	const char* retv = NULL;
	LISTBOX lb;

	// Find matching files
	CHOOSEFILECONTEXT ctx;
	ctx.pattern = pattern;
	ctx.pBufferBase = malloc(MAX_FILENAME_BUFFER);
	ctx.pBufferPos = ctx.pBufferBase;
	ctx.nFiles = 0;
	ctx.ppFiles = malloc(MAX_FILENAMES);
	ctx.pszNullOption = pszNullOption;

	// Setup current directory and selection
	if (selectedFile == NULL)
	{
		ctx.pszDirectory = stralloc("/");
		ctx.pszSelection = NULL;
	}
	else
	{
		const char* pszSlashPos = strrchr(selectedFile, '/');
		if (pszSlashPos)
		{
			int len = pszSlashPos - selectedFile;
			char* psz = malloc(len + 1);
			memcpy(psz, selectedFile, len);
			psz[len] = '\0';
			ctx.pszDirectory = psz;
			ctx.pszSelection = pszSlashPos+1;
		}
		else
		{
			ctx.pszDirectory = stralloc("/");
			ctx.pszSelection = NULL;
		}
	}

	int sel = choose_file_load_content(&ctx);

	free(ctx.pszDirectory);
	ctx.pszSelection = NULL;
 
	// Display picker...
	memset(&lb, 0, sizeof(lb));
	lb.window.rcFrame.left = 2;
	lb.window.rcFrame.top = 2;
	lb.window.rcFrame.width = 27;
	lb.window.rcFrame.height = 12;
	lb.window.attrNormal = MAKECOLOR(COLOR_WHITE, COLOR_BLUE);
	lb.window.attrSelected = MAKECOLOR(COLOR_BLACK, COLOR_YELLOW);
	lb.window.title = pszTitle;
	lb.window.wndProc = wndProc;
	lb.window.user = &ctx;
	lb.selectedItem = sel;
//	listbox_set_data(&lb, ctx.nFiles, ctx.ppFiles);
	lb.getItemCount = choose_file_getItemCount;
	lb.getItemText = choose_file_getItemText;
	lb.scrollPos = listbox_ensure_visible(&lb, lb.selectedItem);

	sel = window_run_modal(&lb.window);

	// If same file, cancel
	const char* pszFile = ctx.pszSelection;
	if (pszFile && selectedFile && strcmp(pszFile, selectedFile)==0)
	{
		free(pszFile);
		pszFile = NULL;
	}


done:
	// Clean up our buffers
	free(ctx.pBufferBase);
	free(ctx.ppFiles);

	// Return 
	return pszFile;
}	


// Choose a file matching pattern.
// Returns NULL if cancelled, empty string if ejected, otherwise fully qualified file name
// which must be free()d when finished with
const char* choose_file(const char* pattern, const char* selectedFile, const char* pszNullOption)
{
	return choose_file_ex(pattern, selectedFile, pszNullOption, "Choose File", choosefile_wndproc);
}

