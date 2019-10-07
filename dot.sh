
#!/usr/bin/env bash

# Dit script zet alle *.dot bestanden in een folder om naar de overeenkomstige *.png bestanden.
# Dit is een linux script, het kan ook gerunned worden op Windows als Cygwin of MinGW is geinstalleerd.
# Indien dit niet het geval is, kan je het dot commando manueel uitvoeren als volgt:
#     dot voorbeeld.dot -Tpng -o voorbeeld.png
# Op de labo computers heb je vaak geen rechten om iets te installeren. Installeer daarom de zip-versie:
# Deze moet niet geinstalleerd worden. Je kan de locatie van dot.exe toevoegen aan de PATH variabele of
# aanroepen met C:\...\dot.exe

for file in *.dot ; do
	TARGET_FILE="${file%.*}.png"
	echo "Converting ${file} -> ${TARGET_FILE} ..."
	dot $file -Tpng -o $TARGET_FILE
done

echo "Done"
