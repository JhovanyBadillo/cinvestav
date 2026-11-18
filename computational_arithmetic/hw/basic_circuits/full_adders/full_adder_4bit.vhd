library ieee;
use ieee.std_logic_1164.all;

entity full_adder_4bit is
    port (
        A, B  : in std_logic_vector(3 downto 0);
        c_in  : in std_logic;
        S     : out std_logic_vector(3 downto 0);
        c_out : out std_logic
    );
end entity;

architecture rtl of full_adder_4bit is
    signal c_in1, c_in2, c_in3 : std_logic;
begin
    fa_bit0_unit : entity work.full_adder_1bit(rtl)
        port map(
            a => A(0),
            b => B(0),
            c_in => c_in,
            sum => S(0),
            c_out => c_in1
        );
    fa_bit1_unit : entity work.full_adder_1bit(rtl)
        port map(
            a => A(1),
            b => B(1),
            c_in => c_in1,
            sum => S(1),
            c_out => c_in2
        );
    fa_bit2_unit : entity work.full_adder_1bit(rtl)
        port map(
            a => A(2),
            b => B(2),
            c_in => c_in2,
            sum => S(2),
            c_out => c_in3
        );
    fa_bit3_unit : entity work.full_adder_1bit(rtl)
        port map(
            a => A(3),
            b => B(3),
            c_in => c_in3,
            sum => S(3),
            c_out => c_out
        );
end rtl;