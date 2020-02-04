--------------------------------------------------------------------------
--
-- SysConVideoController
--
-- Implements a simple overlay video panel
-- 
-- Copyright (C) 2019 Topten Software.  All Rights Reserved.
--
--------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity SysConVideoController is
port
(
	-- Clocking
	i_reset : in std_logic;
    i_clock : in std_logic;
    i_clken : in std_logic;

	-- VGA connection
	i_horz_pos : in STD_LOGIC_VECTOR(10 downto 0);
	i_vert_pos : in STD_LOGIC_VECTOR(10 downto 0);
	o_red: out STD_LOGIC_VECTOR(1 downto 0);
	o_green: out STD_LOGIC_VECTOR(1 downto 0);
	o_blue: out STD_LOGIC_VECTOR(1 downto 0);
	o_transparent : out std_logic;

	-- Video Controller
	o_video_ram_addr : out STD_LOGIC_VECTOR(8 downto 0);
	i_video_ram_char : in STD_LOGIC_VECTOR(7 downto 0);
	i_video_ram_color : in STD_LOGIC_VECTOR(7 downto 0)
);
end SysConVideoController;

architecture Behavioral of SysConVideoController is
	signal s_pixel_in_range : std_logic;
	signal s_pixel : std_logic;
	signal s_color_nibble : std_logic_vector(3 downto 0);
	signal s_current_x_coord : std_logic_vector(10 downto 0);
	signal s_upcoming_x_coord : std_logic_vector(10 downto 0);
	signal s_current_char_column : std_logic_vector(6 downto 0);
	signal s_current_char_row : std_logic_vector(6 downto 0);
	signal s_upcoming_char_column : std_logic_vector(6 downto 0);
	signal s_current_xpos_in_char : std_logic_vector(2 downto 0);
	signal s_current_ypos_in_char : std_logic_vector(4 downto 0);
	signal s_pixel_in_x_range : std_logic;
	signal s_pixel_in_y_range : std_logic;
	signal s_character_bitmap : std_logic_vector(7 downto 0);
	signal s_current_pixel : std_logic;
	signal s_charrom_addr : std_logic_vector(10 downto 0);
	signal s_charrom_dout : std_logic_vector(7 downto 0);
	signal s_vram_addr_upcoming : std_logic_vector(8 downto 0);
	signal s_color_delayed : std_logic_vector(7 downto 0);
	constant c_vga_width : integer := 800;
	constant c_vga_height : integer := 600;
	constant c_vga_timing_width : integer := 1056;
begin

	-------------- VIDEO GENERATION ---------------

	process (i_clock)
	begin
		if (rising_edge(pixel_clock)) then
			s_color_delayed <= i_video_ram_color;
		end if;
	end process;

	-- Count the vertical scan/character lines
	process (i_clock)
	begin
		if rising_edge(pixel_clock) then
			if i_reset='1' then

				s_current_ypos_in_char <= (others=>'0');
				s_current_char_row <= (others=>'0');

			elsif i_clken = '1' then

				-- Pick one s_pixel somewhere in the back porch to trigger line counter
				if i_horz_pos = std_logic_vector(to_unsigned(VGA_TIMING_WIDTH-10, 11)) then

					if unsigned(i_vert_pos) = 0 then					-- top

						-- First s_pixel row
						s_current_ypos_in_char <= (others=>'0');
						s_current_char_row <= (others=>'0');

					else

						-- Increment our row counters
						if (s_current_ypos_in_char = "01011") then
							s_current_ypos_in_char <= (others=>'0');
							s_current_char_row <= std_logic_vector(unsigned(s_current_char_row)+1);
						else
							s_current_ypos_in_char <= std_logic_vector(unsigned(s_current_ypos_in_char) + 1);
						end if;

					end if;

				end if;

			end if;
		end if;
	end process;


	-- Calculate the current and upcoming microbee x coordinate
	s_current_x_coord <= std_logic_vector(unsigned(i_horz_pos) - (c_vga_width-256));			-- right
	s_upcoming_x_coord <= std_logic_vector(unsigned(s_current_x_coord)+2);

	-- Calculate the current and upcoming horizontal character char number (xcoord / 8)
	s_current_char_column <= s_current_x_coord(9 downto 3);
	s_upcoming_char_column <= s_upcoming_x_coord(9 downto 3);

	-- Calculate the current position within the char (xcoord % 8)
	s_current_xpos_in_char <= s_current_x_coord(2 downto 0);

	-- Work out if the current s_pixel is "onscreen"
	s_pixel_in_x_range <= '1' when unsigned(s_current_char_column) < 32 else '0';
	s_pixel_in_y_range <= '1' when unsigned(s_current_char_row) < 16 else '0';

	-- Work out the memory address of the upcoming char
	s_vram_addr_upcoming <= s_current_char_row(3 downto 0) & s_upcoming_char_column(4 downto 0);

	-- The upcoming vram address is the one we need to request now
	o_video_ram_addr <= s_vram_addr_upcoming;
	
	-- Setup Character Rom lookup address.
	s_charrom_addr <= i_video_ram_char(6 downto 0) & s_current_ypos_in_char(3 downto 0);
	
	-- Work out the current s_pixel value
	s_current_pixel <= s_charrom_dout(to_integer(unsigned(not s_current_xpos_in_char)));
 				
	-- Is the s_pixel within range
	s_pixel_in_range <= s_pixel_in_x_range and s_pixel_in_y_range;
	o_transparent <= '0' when s_pixel_in_range='1' and (s_current_pixel='1' or s_color_delayed(3 downto 0)/=s_color_delayed(7 downto 4)) else '1';

	-- Select the appropriate half of the color byte
	s_color_nibble <= s_color_delayed(3 downto 0) when (s_current_pixel='1') else s_color_delayed(7 downto 4);

	o_red <= s_color_nibble(0) & s_color_nibble(3);
	o_green <= s_color_nibble(1) & s_color_nibble(3);
	o_blue <= s_color_nibble(2) & s_color_nibble(3);

	-- SysCon Character ROM
	charrom : entity work.SysConCharRom
	PORT MAP 
	(
		i_clock => pixel_clock,
		i_addr => s_charrom_addr,
		o_dout => s_charrom_dout
	);

end Behavioral;

