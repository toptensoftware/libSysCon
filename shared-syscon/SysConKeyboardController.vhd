--------------------------------------------------------------------------
--
-- SysConKeyboardController
--
-- Implements the syscon keyboard controller
-- 
-- Copyright (C) 2019 Topten Software.  All Rights Reserved.
--
--------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity SysConKeyboardController is
port
(
	-- Clocking
	i_reset : in std_logic;
    i_clock : in std_logic;

	-- CPU connection
	i_cpu_port_number : std_logic;
	i_cpu_port_rd_falling_edge : in std_logic;
	o_cpu_din : out std_logic_vector(7 downto 0);

	-- IRQs
	o_irq : out std_logic;		-- anytime anything in receive queue

	-- UART
    i_key_scancode : in std_logic_vector(7 downto 0);
    i_key_released : in std_logic;
	i_key_available : in std_logic;

	-- Keyboard control state
	i_is_syscon_key : in std_logic;		-- '1' if key should be accepted even if i_all_keys = '0'
	i_key_modifiers : in std_logic_vector(1 downto 0);
	i_all_keys : in std_logic			-- when '1' all keys will be captured
);
end SysConKeyboardController;

architecture Behavioral of SysConKeyboardController is
	signal s_have_key : std_logic;
	signal s_current_key : std_logic_vector(7 downto 0);
	signal s_current_key_release : std_logic;
	signal s_current_key_modifiers : std_logic_vector(1 downto 0);
begin

	o_cpu_din <=  s_current_key when i_cpu_port_number = '0' else 
					s_current_key_release & "00000" & s_current_key_modifiers;
	o_irq <= s_have_key;

	exec : process(i_clock)
	begin
		if rising_edge(i_clock) then
			if i_reset = '1' then
				s_current_key <= (others => '0');
				s_have_key <= '0';
				s_current_key_release <= '0';
				s_current_key_modifiers <= (others => '0');
			else
				if i_key_available = '1' and (i_is_syscon_key = '1' or i_all_keys = '1') then
					
					-- Capture the key
					s_have_key <= '1';
					s_current_key <= i_key_scancode;
					s_current_key_release <= i_key_released;
					s_current_key_modifiers <= i_key_modifiers;

				elsif i_cpu_port_rd_falling_edge = '1' and i_cpu_port_number = '1' then

					-- Clear captured key now that it's been read
					s_current_key <= (others => '0');
					s_have_key <= '0';
					s_current_key_release <= '0';
					s_current_key_modifiers <= (others => '0');

				end if;

			end if;
		end if;
	end process;


end Behavioral;
