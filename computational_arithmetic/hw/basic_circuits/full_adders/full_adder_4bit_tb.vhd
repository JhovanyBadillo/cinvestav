library ieee;
use ieee.std_logic_1164.all;

entity full_adder_4bit_tb is
end entity;

architecture sim of full_adder_4bit_tb is
    signal A, B, S     : std_logic_vector(3 downto 0);
    signal c_in, c_out : std_logic;
begin
    uut : entity work.full_adder_4bit(rtl)
        port map(
            A => A,
            B => B,
            c_in => c_in,
            S => S,
            c_out => c_out
        );
    process
    begin
        -- nothing exhaustive
        A <= "1001"; B <= "0111"; c_in <= '0'; wait for 10 ns;
        A <= "1101"; B <= "0010"; c_in <= '0'; wait for 10 ns;
        wait;
    end process;
end architecture;