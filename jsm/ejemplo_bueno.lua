-- Added on sept. 28th, 2005

-- In this simple example, data is collected from a CAEN C414 and written to a remote file through NFS (Network File System).

-- NFS is mounted like in any Linux system; the mnt_cmd string contains the complete mount command: the remote filesystem which has the path /root/jenet will be seen by the CAMAC controller in the nfs folder. The mnt_cmd string is -- then passed to the execute command that performs the actual mount operation. It is important to remember that, on a Linux  computer hosting the NFS filesystem, the following line must be added to /etc/exports file:

--   <nfs_directory> <ind.IP controllore> rw, all_squash

-- that in the case of this specific example will be

--   /root/jenet    193.206.85.227  rw, all_squash

-- TDC read operation is conditioned to a LAM event arrival, by means of the CCLWT command that puts the Lua interpreter on hold while waiting for a LAM event from the specified slot.

-- ***********************************************************************************************************************************************************************************

-- definiton of labels for inputs 1-4
-- In this example C414 is in slot 14 (slots are numbered 1 to 23)
slot = 14

-- In this example we use inputs 1,2,3,4.
-- (inputs on C414 are numbered 0 to 7,
-- on front panel from 1 to 8 !!)
inp1 = 0
inp2 = 1
inp3 = 2
inp4 = 3

-- miscellaneous parameters
endl = '\n'

-- define NFS mount command
mnt_cmd = "mount -t nfs -o nolock 193.206.85.227:/root/jenet /nfs"

-- mount NFS file
execute(mnt_cmd)

-- open file
f = openfile("/nfs/jsm/script/dati.raw","w")

-- crate initialization and clear
CCCC()
CCCZ()

-- infinite loop to read TDC
while (1) do
  doevents();
  
  -- clear C414
  CSSA(9,slot,0,0)

  -- enable LAM
  CSSA(26,slot,0,0)
 
  -- Wait for TDC LAM
  CCLWT(slot)
 
  -- read input values
  q,inp1_value = CSSA(0,slot,inp1,0);
  q,inp2_value = CSSA(0,slot,inp2,0);
  q,inp3_value = CSSA(0,slot,inp3,0);
  q,inp4_value = CSSA(0,slot,inp4,0);
  
  -- LAM Acknowledge
  LACK(slot)

  -- write to NFS file
  z=write(f,inp1_value," ",inp2_value," ",inp3_value," ",inp4_value,endl)
 
end
closefile(f)
