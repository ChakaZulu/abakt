Instructions:

Install AurigaDoc (instructions can be found on http://aurigadoc.sourceforge.net/ ).

Open the file 'settings.bat' in the directory where this file (README.txt) is located.
- Set AURIGADOC_HOME to the path where AurigaDoc is installed on your system.
- Set ABAKT_MANUAL_LANGUAGE the locale id for the language your of your translation. Default is English (en).

Review the changes in the original English manual ("en\abakt_manual.txt") by comparing it with the previous manual you used to create your translation (ie. "en\abakt_manual-0.9.2-RC3.xml"). Update your translation accordingly.
You can use a diff tool like WinMerge to compare the files (http://winmerge.sourceforge.net/ ).

Execute 'create_ALL_abakt_manuals.bat' or one of the 'create_*_abakt_manual.bat' files to generate Abakt's manual.

----------------------

Note:
It is possible to see a live preview of abakt_manual.xml in your browser.
Read LIVE_PREVIEW_HOWTO.txt for instructions.
