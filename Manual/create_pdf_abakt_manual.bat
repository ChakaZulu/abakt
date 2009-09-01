@echo.
@echo ====================================================
@echo == PDF:
@echo ====================================================
@echo.

@set OUTPUT_DIR=pdf

@rem # Read settings (AurigaDoc location and default language):
@call settings.bat


@if exist %ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR% (
	@rd /S /Q "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%"
)

@md "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%"

@rem # AurigaDoc PDF requires the xml file and the resource files in one directory:
@copy "%ABAKT_MANUAL_LANGUAGE%\abakt_manual.xml" "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%\*"
@copy res\* "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%\*"

@rem # AurigaDoc wants forward slashes:
@call %AURIGADOC_HOME%\bin\aurigadoc.bat -pdf -XML "%ABAKT_MANUAL_LANGUAGE%/%OUTPUT_DIR%/abakt_manual.xml" -OUT "%ABAKT_MANUAL_LANGUAGE%/%OUTPUT_DIR%/Abakt.pdf"

@rem # Rename/replace Abakt.pdf file and delete all temporary files (using an awkward method):
@md "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%-temp"
	@copy "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%\Abakt.pdf" "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%-temp\Abakt.pdf"
	@rd /S /Q "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%"
@md "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%"
	@copy "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%-temp\*" "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%\*"
	@rd /S /Q "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%-temp"

@echo ----------------------------------------------------
@echo.
