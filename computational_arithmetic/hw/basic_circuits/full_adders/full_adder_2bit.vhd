library ieee;
use ieee.std_logic_1164.all;

entity full_adder_2bit is
    port (
        A, B   : in std_logic_vector(1 downto 0);
        c_in0  : in std_logic;
        S      : out std_logic_vector(1 downto 0);
        c_out1 : out std_logic
    );
end entity;

architecture rtl of full_adder_2bit is
    signal c_in1 : std_logic;
begin
    fa_bit0_unit : entity work.full_adder_1bit(rtl)
        port map(
            a => A(0),
            b => B(0),
            c_in => c_in0,
            sum => S(0),
            c_out => c_in1
        );
    fa_bit1_unit : entity work.full_adder_1bit(rtl)
        port map(
            a => A(1),
            b => B(1),
            c_in => c_in1,
            sum => S(1),
            c_out => c_out1
        );
end rtl;
