@echo off

@rem # Where did you install AurigaDoc?
@set AURIGADOC_HOME="P:\aurigadoc"

@rem # Set the locale id for the language your of your translation. Default is English (en).
@rem # Example: to create a Dutch (nl) translation, replace ABAKT_MANUAL_LANGUAGE=en with ABAKT_MANUAL_LANGUAGE=nl.
@if NOT defined ABAKT_MANUAL_LANGUAGE (
	set ABAKT_MANUAL_LANGUAGE=en
)
@echo - Language: %ABAKT_MANUAL_LANGUAGE%
@echo.
