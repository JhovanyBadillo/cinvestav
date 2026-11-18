library ieee;
use ieee.std_logic_1164.all;

entity full_adder_1bit_tb is
end entity;

architecture sim of full_adder_1bit_tb is
    signal a, b, c_in : std_logic;
    signal sum, c_out : std_logic;
begin
    uut : entity work.full_adder_1bit
        port map (a => a, b => b, c_in => c_in, sum => sum, c_out => c_out);

    process
    begin
        a <= '0'; b <= '0'; c_in <= '0'; wait for 10 ns;
        a <= '0'; b <= '0'; c_in <= '1'; wait for 10 ns;
        a <= '0'; b <= '1'; c_in <= '0'; wait for 10 ns;
        a <= '0'; b <= '1'; c_in <= '1'; wait for 10 ns;
        a <= '1'; b <= '0'; c_in <= '0'; wait for 10 ns;
        a <= '1'; b <= '0'; c_in <= '1'; wait for 10 ns;
        a <= '1'; b <= '1'; c_in <= '0'; wait for 10 ns;
        a <= '1'; b <= '1'; c_in <= '1'; wait for 10 ns;
        wait;
    end process;
end architecture;