COPY %1\constraintsEngineLibrary.dll X:\constraintsEngine\dll_%2\bin\constraintsEngineLibrary.dll
COPY %1\constraintsEngineLibrary.lib X:\constraintsEngine\dll_%2\bin\constraintsEngineLibrary.lib

DEL /S X:\constraintsEngine\dll_%2\include\*.h
XCOPY /S /Y X:\constraintsEngine\constraintEngine\*.h X:\constraintsEngine\dll_%2\include