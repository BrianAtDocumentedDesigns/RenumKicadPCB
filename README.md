# RenumKicadPCB
Utility which geographically renumbers Kicad PCB reference designations then back-annotates the schematic

This program automatically renumbers a KiCad PCB according to various rules (see options below).
It then back-annotates the renumbering scheme into the schematic hierarchy.

This makes finding components on the PCB very easy.


V0.205	Fixed bug reported by Sean Happel associated with PullFieldString function 
		(Component fields L0, etc.,  needed to be found at beginning of line
		RenumKiCadPCB now updates the netlist file and creates a backup

V0.300	Do all changes in memory before writing to files.
		If there are warnings, ask whether to commit changes
  		Then backup all files, write out new ones.
  		Fixed ability to have a non-file extent "." i.e TERES_PCB1-A64-MAIN_Rev.C.pro
  		Fixed a number of bugs associated with/exposed by weird file content in TERES_PCB1-A64-MAIN_Rev.C.pro

V0.305  This is a version intended toward incorporating the function in Kicad V6.
        - removed the command line function.
        - renamed the menu entry an added the ability to define different PCB, Schematic, and Netlist files
        - added the ability to remove prepends (see below)
        - moved to a more object style of argument passing
        - removed automatic logging of locations, changes, etc.. You manually edit renumparameters.txt
          and add a line
          
            Log=n
            where = 01 means log the coordinates
                    02 means log the change plan
                    04 means log the changes (i.e. all the nets, etc)
                    
                    You can use these in combination so 06 means log change plan + changes
       
        
		
Note: there may be other bugs!

I have stress tested with 
https://github.com/OLIMEX/DIY-LAPTOP/tree/master/HARDWARE/A64-TERES/TERES-PCB1-A64-MAIN_Rev_C
which is the most complex KiCad project I have come across.

The Rev C files themselves have issues:
ERC and DRC show errors. I was able to fix all the ERC errors
	1) Resistor networks in the schematic have sections with different modules than the PCB
	2) There is an excess PWR_FLAG on a power connector.
	
	in addition (bug reported to the developers) LCD.SCH has a "phantom" instantiation which does 
	not show up in ERC. The phantom instantiation meant parts were labeled U? but didn't show up 
	as unannotated. Even though renumKiCadPCB works, it flags errors such as this.
	
	Also there are blank reference designations (i.e. "") on the PCB.
	
	The good news is, all these errors exposed issues with RenumKiCadPCB!
	
	The PCB also fails DRC with 9 errors as distributed. After creation and importation of a 
	netlist this becomes 27 errors. The new errors appear to be associated with zones, a well
	established KiCad bug (see below).
	
	I couldn't figure out Olimex's license, otherwise I'd post the fixed files (renumbered!)
	
	After RenumKiCadPCB is run on the original files or the fixed files the DRC errors are the same,
	respectively. I assume, therefore, that RenumKiCadPCB is working. (famous last words ...)
	
	
		
The Kicad "pour error" described below still seems to be present in Kicad V5 nightly builds. The
way you fix it is to "Edit" the zone (which I find a pain in the butt to do - type "E" *exactly* ones
zone outline) make sure Net Filtering Display: Show All is selected and rename the zone net.

Also, it seems to me the best time to run RenumKiCadPCB is prior to routing the board rather than just 
before making the Gerbers, which is what I have been doing. That way any missing component issues will be
resolved early. No sense routing a board if the schematic and PCB have different parts!

If there is anybody out there willing to do encourage the KiCad development group, please do. 

Similarly, I would welcome anybody with requisit c++ or GUI programming skills to put a wrapper on the 
code.

PLEASE LET ME KNOW OF ANY BUGS!
		
**** NOTE: If you get strange "out of order" results change the grid setting (usually higher works - to a point) ****

Linux make instructions

This should work for most versions of Linux but I tried it under Ubuntu

1) Create a directory (i.e. RenumKiCadPCB) and extract all the archive files into it. 
2) Delete RenumKiCadPCB.exe (the Windows executable)
3) Type make RenumKiCadPCB
4) You should now have an executable called RenumKiCadPCB. Under Ubuntu it is already executable.
5) Execute as ./RenumKiCadPCB

Jano reports
 The make RenumKiCadPCB don’t works out of the box, instead I was able to compile with: 
 gcc RenumKiCadPCB.c -o RenumKiCadPCB -lm



Instructions
1) Archive your KiCad project (just in case)
2) Verify and record/correct any schematic or DRC PCB errors
3) Run this program (RenumKiCadPCB)
4) Type in the input file name and set any other options. The maximum file name is 2048 characters 
   and if it contains spaces (i.e. this is a file) you probably need to surround it with ""
5) Open the project and create a netlist
6) Open the PCB and import the netlist 
	**** The first time you do this PCBNew will recreate the connectivity database
7) Run DRC and verify there are no new error, no connects, etc.

Note on the use of prepends
1) I have added the ability to remove prepends: if you ran, for example a bottom prepend of B_
(which causes all bottom side ref des to start with B_, you can run -B_ to remove it.

So prepend B_ causes bottom side C1 to become B_C1. Run with -B_ makes B_C1 into C1

It is important to note that B_C1 is not the same as C1 so running a prepend will result in 
bottom side renumbering starting at 1. You can fix this by setting the bottom start ref des to the 
number the top ended at. So if your last front ref des was C99, set the bottom prepend to B_ and 
the bottom start refdes to 100. Then the first capacitor on the bottom will be B_C100.


								****** KNOWN ISSUE ****** 
Due to a bug in KiCad PCBNew ([Bug 1609401] Re: PCBnew fails to properly import netlist after changing 
annotation with pours see https://bugs.launchpad.net/kicad/+bug/1609401) 

If RenumKiCadPCB is run on a project with copper pours and a schematic netlist is created and imported
into PCBNew errors can occur in the netlist import and subsequent DRC. 

This happens even without using RenumKiCadPCB: sometimes after changing reference designations on 
PCB and eeSchema if you then generate a netlist and re-import it into PCBNew there may be netlist 
errors and DRC errors if the board has copper pours.

The fix is simple. Type 'B' and the pours will regenerate except where there are errors where they
appear hatched. Note the rats nest netlist name in the hatched error zone. Edit the zone (hit 'E' 
near a zone edge) and change the pour net to the rats net netlist name. (the rats nest name is usually
near the top of the selection window). Type 'B' again, and the pour will fill. Run DRC and the errors 
are gone.
	
I have filed a bug report with the KiCad developers regarding this issue.

**************************************************************************************************


RenumKiCadPCB Options 

V0.2 and after works on as a command line tool or a rudimentary menu based user interface.

Simply run RenumKiCadPCB and type in the input file at the prompt. When all the parameters are correct,
type "r" to run.


