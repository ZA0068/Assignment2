----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 12/01/2022 04:45:13 PM
-- Design Name: 
-- Module Name: counter - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity counter is
    Port ( clk : in STD_LOGIC;
           rst : in STD_LOGIC;
           counter_out : out STD_LOGIC_VECTOR (24 downto 0);
           active : out STD_LOGIC);
end counter;

architecture Behavioral of counter is

signal sign_counter :   UNSIGNED(24 downto 0) := (OTHERS =>'0');
signal sign_active :   STD_LOGIC := '0';
begin
mainProcess: process(clk,rst)
begin
    if (rst='1') then
        sign_counter <= (OTHERS =>'0');
        sign_active <= '0';
    elsif(rising_edge(clk)) then
        if(sign_active =  '0') then
            sign_active <= '1';
        end if;
        sign_counter <= sign_counter + 1;
    end if;
    counter_out <= STD_LOGIC_VECTOR(sign_counter);
    active <= sign_active;
end process ; -- mainProcess

end Behavioral;
