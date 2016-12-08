#################################################################
       Desktop Mascots Program for UNIX/MS Windows
       macopix : Mascot Constructive Pilot for X                 
                                                    ver 1.7.3    
                 Quick document for Windows(binary) version
                                                                 
                            2008.05.24  Kurumi Chimari           
                            chimari@rosegray.sakura.ne.jp        
                            http://rosegray.sakura.ne.jp/        
#################################################################

-------------------------------------------------------------------------------
[ Name         ] MaCoPiX-1.7.3-win32
[ Developer    ] Kurumi Chimari
[ URL          ] http://rosegray.sakura.ne.jp/
[ e-mail       ] chimari@rosegray.sakura.ne.jp
[ OS Types     ] Microsoft Windows 2000 Professional / XP 
[ Type of Soft ] Desktop Mascot Software (Freeware following GPL2)
[ Copying      ] GNU GPL2 (See Doc\COPYING)
[ Copyright    ](C) 2002-2008 Kurumi Chimari
-------------------------------------------------------------------------------

=============================================================
                      - Contents -
  - About this program
  - Main features
  - System requirement
  - Windows version
  - Installation - Starting up
  - Installation of mascots
  - Biff function
  - FAQ
  - Building windows version for source tar-ball
  - Copyright
=============================================================


## About this program
  MaCoPiX is a desktop mascot program.
  This program is a free software distributed under the GNU GPL2.

  Currently, you can make following types of mascots using MaCoPiX.
         - Focus follower (Window sitters : likely ActX)
         - Fix style      (Desktop wappen?)
  Furthermore, you can select with or without a digital clock for
  each types of mascots. So, MaCoPiX can be used as a sort of desktop clock
  applications. (But the clock function could be still poor.)
  And, the biff function is also available for POP/APOP environments.
  SSL/TLS protocol for POP/APOP has been supported experimentally in
  ver.1.6.2.

  At once, users can create mascots and change their settings from GUI
  instead of editing mascot files directly.
  

## Main features
  - Focus follower / Fixed desktop mascots
  - Biff function [POP3/APOP]
  - Clock function
  - Detailed and varied settings fro GUI (Mascot can be build from GUI)
  - Multiple execution


## Environment
   [for Microsoft Windows (binary version)]
     - Windows2000,XP or later (Still unconfirmed on Vista)
     - Including TAR32.DLL(GPL) in the binary distribution.
        * You can download it (free-software, GPL) form the HP of
          "Common Archiver Library Project".
           URL  http://www.csdinc.co.jp/archiver/index-e.html
     - All required libraries (Gtk+ etc.) are also included in the
       binary package. 


## Windows version
    MaCoPiX has a common source code for Windows and UNIX.
    Current windows version does not include 
     "Socket Message function (includes duet message mode)" .
    But, windows version has some original features.
      - Automatic extraction of mascot tar+gz archive using "TAR32.DLL".
      - Alpha blending transparency (balloon, clock, mascots, biff image)
         [Currently, ver1.7.0 starts to support transparency also on UNIX
          only with some compositing window managers which supoort to
          render translucent windows.]

    For windows Biff function is only for POP/APOP. (not using any
    local spools).


    All required libraries are included in this binary archive.
    Please copy the whole extracted folders and use it.
    If you have installer version, please use installer.



## Installation - Starting up
    In anyway, you have to download one mascot file at least.
    Without mascot files, you cannot do anything with MaCoPiX.

  1. Download main program of MaCoPiX-win32(this archive)
  2. Install main program
  3. Download MaCoPiX mascot archive (anyone you like)
         (MaCoPiX-mascot-HxB-0.10.tar.gz etc..)
  4. Start up macopix.exe
  5. Install mascots. From "MaCoPiX Starting Up" message window
         "Install" -> "Launcher menu"
     Appoint downloaded mascot archive (.tar.gz)
  6. Mascots are installed automatically
     (UserData\ directory automatically created in the program directory)


## Installation of Mascots
  1. Download MaCoPiX mascot archive (anyone you like)
         (MaCoPiX-mascot-HxB-0.10.tar.gz etc..)
  2. Start up macopix.exe
  3. From right click menu of mascot
     "Install" -> "Launcher Menu" 
     Appoint downloaded mascot archive (.tar.gz)
  6. Mascots are installed automatically
     (to UserData\ directory in the program directory)


## BIFF function
  This function is still testing version for Windows.
  Please be careful to use it.

  Basically, windows does not use any local spool.
  So, the biff function of MaCoPiX-win32 can check only POP/APOP
  server.  (default : every 60secs)
  POP/APOP with SSL/TLS protocols are now supported experimentally.
  MaCoPiX-win32 cannot access IMAP servers.

  In default, your password for the POP server will be filed
  in the setting (UserData/macopix.ini).
  If you pay attention the security on your PC, please uncheck
  "Save password" in the Biff setting window.
  In this case, you have to enter password for POP server at 
  starting up of MaCoPiX POP function, but more secure.  


## FAQ
  TBD



## Building MaCoPiX-Win32 from source tar-ball
   msys,mingw,Gtk+/Win32, and TAR32.DLL are required.
   You can also use UNLHA32.DLL. If you need, install it.
   You can refer to a MUA program "sylpheed" page, which
   also has the UNIX/Win common source code.
      URL: http://sylpheed.sraoss.jp/wiki/

   - Extract source archive to the user directory on msys sytem.
   - Prepare UNLHA32.DLL (If you need)
     Copy 
        UNLHA32.DLL      (C:\Program Files\ArchiverDll\UNLHA32\)
        UNLHA32.H
     to a directory on msys.
      % pexports UNLHA32.DLL > unlha32.def
     Edit unlha32.def .  Add stdcall stack length (@n)
              Unlha  -> Unlha@16
        UnlhaCloseArchive  ->  UnlhaCloseArchive@4
        UnlhaOpenArchive ->  UnlhaOpenArchive@12
        UnlhaFindFirst  ->  UnlhaFindFirst@12
      % dlltool -d unlha32.def -l libunlha32.a
   - Copy libunlha32.a & UNLHA32.H to src/ directory in the source tree.
     (For TAR32.DLL, the save procedure is required. But already prepared
      in the distributed archive.)
   - Run the script for make in the source directory
      % ./makewin32.sh
   - Program files are built in $HOME/dist/ .
     


## Copyright, Copying
   The code in this distribution is Copyright 2002-2008 by Kurumi Chimari.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

   In addition, as a special exception, K.Chimari gives permission to link 
   this code of this program with the OpenSSL library (or with modified
   versions of OpenSSL that use the same license as OpenSSL), and distribute
   linked combinations including the two. You must obey the GNU General 
   Public License in all respects for all of the code used other than OpenSSL.
   If you modify this file, you may extend this exception to your version of 
   the file, but you are not obligated to do so. If you do not wish to do so,
   delete this exception statement from your version.


   GLib,GTK+,libiconv libraries are free software distributed under the
   LGPL. Please see COPYING.LIB for more details.

   OpenSSL libraries is distributed under the Open SSL and SSLeay licenses.
   Please see LICENSE.SSL for more details.

   TAR32.DLL is free software distributed under the GPL, 
   developed by Mr. Tsuneo Yoshioka.
     Please see URL http://www.csdinc.co.jp/archiver/lib/tar32.html


