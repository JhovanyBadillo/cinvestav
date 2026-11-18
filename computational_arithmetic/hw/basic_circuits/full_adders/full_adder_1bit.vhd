library ieee;
use ieee.std_logic_1164.all;

entity full_adder_1bit is
  port (
    a     : in  std_logic;
    b     : in  std_logic;
    c_in  : in  std_logic;
    sum   : out std_logic;
    c_out : out std_logic
  );
end entity;

architecture rtl of full_adder_1bit is
begin -- here, intentionally, we use the disjunctive normal form (Sum-Of-Products)
  sum  <= ((not a) and (not b) and c_in) or ((not a) and b and (not c_in)) or (a and (not b) and (not c_in)) or (a and b and c_in);
  c_out <= ((not a) and b and c_in) or (a and (not b) and c_in) or (a and b and (not c_in)) or (a and b and c_in);
end architecture;
