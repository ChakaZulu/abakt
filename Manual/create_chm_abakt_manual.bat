@echo.
@echo =========================================================
@echo == CHM (Compiled HTML Help):
@echo =========================================================
@echo.

@set OUTPUT_DIR=chm

@rem # Read settings (AurigaDoc location and default language):
@call settings.bat


@if exist %ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR% (
	@rd /S /Q "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%"
)

@md "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%"
@copy res\* "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%\*"

@rem # AurigaDoc wants forward slashes:
@call %AURIGADOC_HOME%\bin\aurigadoc.bat -chm -XML "%ABAKT_MANUAL_LANGUAGE%/abakt_manual.xml" -OUT "%ABAKT_MANUAL_LANGUAGE%/%OUTPUT_DIR%"


@rem # Rename/replace Abakt.chm file and delete all temporary files (using an awkward method):
@md "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%-temp"
	@copy "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%\adchm.chm" "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%-temp\Abakt.chm"
	@rd /S /Q "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%"
@md "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%"
	@copy "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%-temp\*" "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%\*"
	@rd /S /Q "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%-temp"

@echo ----------------------------------------------------
@echo.
