library ieee;
use ieee.std_logic_1164.all;

entity basic_tb is
end entity;

architecture sim of basic_tb is
  signal a, b, x : std_logic;
  signal q       : std_logic;
begin
  uut : entity work.basic
    port map (a => a, b => b, x => x, q => q);

  process
  begin
    a <= '0'; b <= '0'; x <= '0'; wait for 10 ns;
    a <= '0'; b <= '0'; x <= '1'; wait for 10 ns;
    a <= '0'; b <= '1'; x <= '0'; wait for 10 ns;
    a <= '0'; b <= '1'; x <= '1'; wait for 10 ns;
    a <= '1'; b <= '0'; x <= '0'; wait for 10 ns;
    a <= '1'; b <= '0'; x <= '1'; wait for 10 ns;
    a <= '1'; b <= '1'; x <= '0'; wait for 10 ns;
    a <= '1'; b <= '1'; x <= '1'; wait for 10 ns;
    wait;
  end process;
end architecture;