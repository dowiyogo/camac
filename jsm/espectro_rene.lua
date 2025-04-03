-- setting Pulse Generator
nim_setpulse(100,5,0)
--Dataway Initialization
jn_led(1,1)
CCCZ()
-- Crate clear
CCCC()
-- Crate inhibit OFF
CCCI(0)
--server initialize
c, err = connect("192.168.1.11", 6666)
endl = '\n'
pause(500)
jn_led(1,0)
jn_led(2,1)
-- enable COMBO 2
nim_enablecombo(2,0)
pause(500)
jn_led(2,0)
-- Set GATE parameters
jn_led(3,1)
M = 100
C = 0
L = 0
D = 2
C = bsl(C,10)
L = bsl(L,13)
D = bsl(D,14)
a = bor(M,C)
b = bor(L,D)
word = bor(a,b)
CFSA(17, 3, 0, word)
pause(500)
jn_led(3,0)
-- Nim Out REG for GATE
-- enable Nim OUT Pulse
jn_led(4,1)
CFSA(26, 5, 0, 0)
-- SET Nim OUT Regs
CFSA(16, 5, 0, 65535)
pause(500)
jn_led(4,0)
jn_led(4,1)
-- enable LAM
CSSA(26, 6, 0, 0)
-- disable LAM
CSSA(24, 7, 0, 0)
pause(500)
jn_led(4,0)
jn_led(1,1)
function readout()
local msg
local address
local slot
local Q
local DATA
slot = 6
Q, DATA = CSSA(2, slot, 3, 0)
msg = DATA;
for address = 4, 11 do
Q, DATA = CSSA(2, slot, address, 0)
msg = msg .. " " .. DATA;
end
slot = 7
for address = 4, 11 do
Q, DATA = CSSA(2, slot, address, 0)
msg = msg .. " " .. DATA;
end
c:send(msg, endl)
end
pause(500)
jn_led(1,0)
while (1) do
doevents();
CCLWT(6)
nim_setouts(4,1)
readout()
nim_setouts(4,0)
LACK(6)
end
