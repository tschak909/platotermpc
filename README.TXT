***********************************************************************
**           Preliminary Documentation for PLATOTerm PC              **
***********************************************************************

NOTE: This documentation is preliminary. Expect errors.

------------------
What is PLATOTerm?
------------------

PLATOTerm is a terminal emulator to access CYBIS services now available
on the Internet utilizing a WIFI Modem, or (soon!) an Ethernet device
supported by a Crynwr Packet Driver.

For the purposes of this documentation. PLATO and CYBIS are interchangeable
names for the same platform.

--------------------------------------------------------------
What services are currently available to access via PLATOTerm?
--------------------------------------------------------------
As of writing this preliminary documentation (September 2018), there are
two major CYBIS systems running. CYBER1.ORG and IRATA.ONLINE.

---------------------------
WHAT IS PLATO? (aka CYBIS?)
---------------------------
(from the PLATO wikipedia page:)

PLATO (Programmed Logic for Automatic Teaching Operations), was the first
generalized computer-asisted instruction system. Starting in 1960, it ran
on the University of Illinois ILLIAC I computer. By the late 1970s, it
supported several thousand graphics terminals distributed worldwide, running
on nearly a dozen different networked mainframe computers. Many modern
concepts in multi-user computing were originally developed on PLATO, including
forums, message boards, online testing, e-mail, chat rooms, picture languages,
instant messaging, remote screen sharing, and multi-player games.

-------------------
What is CYBER1.ORG?
-------------------

CYBER1.ORG is a CYBIS system initially set up in 2004, as a haven for
ex-PLATO users to experience a classic PLATO author experience.

CYBER1.ORG is home to many thousands of classic PLATO lessons and
notesfiles which have been restored from various sources, and have
been made available in the interests of preserving PLATO.

---------------------
What is IRATA.ONLINE?
---------------------

IRATA.ONLINE is a CYBIS system that has been set up for the benefit of
the greater vintage computing community, in the interest to provide
a unique experience that can be accessed on a wide variety of
vintage computers with a bitmapped graphics display. To this end,
IRATA.ONLINE develops PLATOTerm for dozens of platforms, so that they
can access CYBIS systems, as well as provide a community and learning
infrastructure for vintage computing users of all types, in the hopes
that as a cohesive community, something unique can hopefully
emerge.

-----------------------------------------------------------
What is the connection between IRATA.ONLINE and CYBER1.ORG?
-----------------------------------------------------------

CYBER1.ORG and IRATA.ONLINE are independent of one another. With that said,
the reason IRATA.ONLINE and PLATOTerm exist, are because of the efforts of
CYBER1.ORG to not only preserve a running PLATO system, and provide the
necessary information for interested parties to write terminal software
to access CYBIS systems, but also in their effort to produce a publically
available distribution of CYBIS that others who are interested may also
run their own CYBIS installation. IRATA.ONLINE is a direct result of the
public release of this distribution.

-----------------------------------------------------
What do I need to run PLATOTerm and connect to CYBIS?
-----------------------------------------------------

* An IBM Compatible machine

This version of PLATOTerm is intended to run on vintage IBM PC or Compatible
systems running MS-DOS. Modern PC systems can run the PTERM program, which
is available from http://cyber1.org/pterm.asp

* A graphics card

This version of PLATOTerm can run in the following graphics modes:

While PLATOTerm will automatically attempt to select the best available
graphics mode for your hardware, you can explicitly select the following
graphics modes, by specifying the appropriate switch:

+ /CGA (640x200, 2 colors, with 320x200 selectable via /CGALO switch.)
+ /EGA (640x350, 16 colors)
+ /HERCULES (720x350, 2 colors)
+ /MCGA (320x200, 256 colors)
+ /VGAMONO (640x480, 2 colors)
+ /VGA (640x480, 16 colors)
+ /SVGA (640x480, 256 colors, must be explicitly selected, currently.)

Video mode notes

+ Hardware cursor does not currently work on SVGA or HERCULES
+ Fonts are preliminary and need visual clean-up.

* A WIFI modem connected to a COM port

Currently, only serial port support is provided, so you will need a WIFI
modem that attaches to an available COM port. COM ports 1 through 4 are
supported.

An example of a RS232 port WIFI modem can be found here:
http://www.cbmstuff.com/proddetail.php?prod=WiModem232

* A FOSSIL driver

PLATOTerm utilizes a FOSSIL driver for serial communications. The author
currently tests with BNU, which can be acquired here:
http://www.pcmicro.com/bnu/

------------------
Starting PLATOTerm
------------------

PLATOTerm can be started by starting PLATO.COM

Explicit video mode setting can be selected by specifying one of the
video mode switches specified above, in the graphics card section.

DTR will be raised, upon starting the program.

-----------------
Exiting PLATOTerm
-----------------

PLATOTerm can be exited, by pressing ALT-X. DTR will be lowered on exit.

---------------
PLATOTerm Setup
---------------

Pressing F1 at any time will bring up the Setup menu, allowing you to set
communication parameters:

P)ORT - (Set the COM port, COM1-COM4)
B)AUD - Set a Baud rate.
S)AVE - Save Settings
E)XIT - Exit setup and return to the terminal.

PLATOTerm by default, will use the following parameters:
* BAUD: 1200 Baud
* PORT: COM1
* N/8/1 (These are always the case, and can not be changed)

Tip: Once you have set the appropriate communications parameters, type
an AT and press enter, to ensure that the modem is working correctly.

---------------
Using PLATOTerm
---------------

When PLATOTerm starts, you will see the following message:

PLATOTerm READY - Press F1 For Setup - ALT-X TO EXIT.

At this point, the terminal has opened a connection to the modem and
your modem should be able to accept commands, provided that your
communication parameters are set correctly.

PLATOTerm starts in TTY mode. In this mode, characters are passed directly
to the modem, and only plain ASCII is received and displayed.

Typically, you will "dial" into the desired CYBIS system, with a command
such as:

ATDTCYBERSERV.ORG:8005

for CYBER1.ORG, or:

ATDTIRATA.ONLINE:8005

for IRATA.ONLINE.

Once PLATOTerm has received a PLATO mode command (ESC STX), the terminal
will change to PLATO mode, and interpret subsequent data as PLATO mode
and command data.

-------------
Disconnecting
-------------

Once you wish to disconnect, you will need to manually hang up the modem,
by sending +++, waiting three seconds, followed by ATH and pressing
ENTER. (to do: add DTR drop by pressing ALT-H)

--------------
PLATO Keyboard
--------------

PLATO terminals had a lot of very special keys, which must be mapped to
equivalent keys on a PC keyboard. The following table shows the
PLATO and equivent PC keys:

+---------------+-------------------------+
|  PLATO        |  PC                     |
+---------------+-------------------------+
|  ANS          |  CTRL-A                 |
|  BACK         |  CTRL-B                 |
|  BACK1        |  CTRL-SHIFT-B           |
|  COPY         |  CTRL-C                 |
|  COPY1        |  CTRL-SHIFT-C           |
|  DATA         |  CTRL-D                 |
|  DATA1        |  CTRL-SHIFT-D           |
|  EDIT         |  CTRL-E                 |
|  EDIT1        |  CTRL-SHIFT-E           |
|  FONT         |  CTRL-F                 |
|  DIVIDE       |  CTRL-G                 |
|  HELP         |  CTRL-H                 |
|  HELP1        |  CTRL-SHIFT-H           |
|  LAB          |  CTRL-L                 |
|  LAB1         |  CTRL-SHIFT-L           |
|  MICRO        |  CTRL-M                 |
|  NEXT         |  ENTER                  |
|  NEXT1        |  SHIFT-ENTER            |
|  SUPER        |  CTRL-P                 |
|  SUPER1       |  CTRL-SHIFT-P           |
|  SQUARE       |  CTRL-Q                 |
|  ACCESS       |  CTRL-SHIFT-Q           |
|  ERASE        |  CTRL-R                 |
|  ERASE1       |  CTRL-SHIFT-R           |
|  STOP         |  CTRL-S                 |
|  STOP1        |  CTRL-SHIFT-S           |
|  TERM         |  CTRL-T                 |
|  MULTIPLY     |  CTRL-X                 |
|  SUB          |  CTRL-Y                 |
|  SUB1         |  CTRL-SHIFT-Y           |
|  SIGMA        |  CTRL-+                 |
|  DELTA        |  CTRL--                 |
|  ASSIGN       |  ESC                    |
+---------------+-------------------------+

System Keys
-----------

F1    - Setup
ALT-X - Exit PLATOTerm
ALT-H - Hang Up

-------------------
Further information
-------------------

Should you need further information, updates, etc. You can check:

* IRATA.ONLINE website: http://www.irata.online/
* IRATA.ONLINE facebook: http://www.facebook.com/irataonline/

Don't forget to check out http://www.cyber1.org/

The author can be reached on IRATA.ONLINE as thomas cherryhomes / atari

