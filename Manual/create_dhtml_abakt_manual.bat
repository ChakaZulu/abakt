@echo.
@echo =========================================================
@echo == DHTML (HTML with Frames) - Entry point: admain.html
@echo =========================================================
@echo.

@set OUTPUT_DIR=dhtml

@rem # Read settings (AurigaDoc location and default language):
@call settings.bat


@if exist %ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR% (
	@rd /S /Q "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%"
)

@md "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%"
@copy res\* "%ABAKT_MANUAL_LANGUAGE%\%OUTPUT_DIR%\*"

@rem # AurigaDoc wants forward slashes:
@call %AURIGADOC_HOME%\bin\aurigadoc.bat -dhtml -XML "%ABAKT_MANUAL_LANGUAGE%/abakt_manual.xml" -OUT "%ABAKT_MANUAL_LANGUAGE%/%OUTPUT_DIR%"

@echo ----------------------------------------------------
@echo.

