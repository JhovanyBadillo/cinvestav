library ieee;
use ieee.std_logic_1164.all;

entity basic is
    port (
        a : in std_logic;
        b : in std_logic;
        x : in std_logic;
        q : out std_logic
    );
end entity;

architecture rtl of basic is
    signal aux : std_logic;
begin
        aux <= a xor b;
        q <= aux and x;
end architecture;