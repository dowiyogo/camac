-- definiton of labels for inputs 1-4
-- In this example C414 is in slot 14 (slots are numbered 1 to 23)
-- slot of module that should send the LAM
slot = 22
-- miscellaneous parameters
endl = ’\n’
host = "62.148.178.181"
port = 6666
c, err = connect(host, port)
c:timeout(1)
c:send("Starting script", endl)
-- Crate initialization
CCCC()
-- Calibration
-- Q, DATA = CSSA(25, slot, address, 0);
-- readout()
function readout()
local msg = "<"
local address
local Q
local DATA
for slot = 21, 22 do
for address = 0, 11 do
Q, DATA = CSSA(0, slot, address, 0);
msg = msg .. " " .. DATA;
-- msg = msg .. " " .. random(1023);
end
end
c:send(msg, endl)
end
-- Infinite loop to read TDC
-- The order of things is very specific
while (1) do
doevents();
-- Clear
CCCZ()
-- Inhibit off
CCCI(0)
-- enable LAM
CSSA(26, slot, 0, 0)
-- Wait for LAM
CCLWT(slot)
-- LAM Acknowledge
LACK(slot)
-- Inhibit on
CCCI(1)
-- Read out
readout()
end

