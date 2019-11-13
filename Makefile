
#################################### Vars #####################################

# Executable
TARGET=SpaceInvaders

# Fichier lors de l'archivage
EXTRA_FILES=$(wildcard data/*.png) Makefile README
ARCHIVE_NAME=SpaceInvaders

# Compilation
BUILD_DIR=objects

CC=gcc
CFLAGS= -DDEBUG -g -O2 -std=c89 -Wall -W -pedantic -O3 `pkg-config --cflags MLV`
LDFLAGS = `pkg-config --libs-only-other --libs-only-L MLV`
LDLIBS = `pkg-config --libs-only-l MLV`


#################################### Rules ####################################

SOURCE_C=$(wildcard src/*.c) $(wildcard src/menus/*.c)
SAVES=$(wildcard data/*.sho)
SOURCE_H=$(wildcard src/*.h) $(wildcard src/menus/*.h)
OBJECTS =$(patsubst src/%.c, $(BUILD_DIR)/%.o, $(SOURCE_C))
ARCHIVES_DATA=$(EXTRA_FILES) $(SOURCE_C) $(SOURCE_H)

all: build
	

##################################### Help ####################################

help:
	@echo "make build       : Construire le jeu"
	@echo "make run         : Lancer le jeu"
	@echo "make rebuild     : Reconstruire le jeu"
	@echo "make clean       : Suppression du jeu"
	@echo "make cleansave   : Suppression des sauvegardes"
	@echo "make cleanarch   : Suppression des archives"
	@echo "make modules     : Listes des modules"
	@echo "make $(BUILD_DIR)/<module>.o: Compilation d'un module"
	@echo "make cleanall    : Suppression jeu + sauv. + arch."
	@echo "make <zip|targz> : Archivage du jeu"
	@echo "make help        : Liste des recettes"
modules:
	@echo "Modules:"
	@$(patsubst $(BUILD_DIR)/%.o,echo '- %';, $(OBJECTS))

run: build
	@echo "Lancement du jeu..."
	@./$(TARGET)


#################################### Build ####################################

rebuild: clean build
	

build: $(TARGET)
	

$(TARGET): $(OBJECTS)
	@echo "Creation de l'executable..."
	@$(CC) -o $@ $(LDFLAGS) $(OBJECTS) $(LDLIBS) 

$(BUILD_DIR)/menus/%.o:src/menus/%.c src/menus/%.h $(BUILD_DIR)/menus
	@echo "Compilation du menu $*..."
	@$(CC) $(CFLAGS) -c src/menus/$*.c -o $(BUILD_DIR)/menus/$*.o

$(BUILD_DIR)/%.o:src/%.c src/%.h $(BUILD_DIR)
	@echo "Compilation du module $*..."
	@$(CC) $(CFLAGS) -c src/$*.c -o $(BUILD_DIR)/$*.o

$(BUILD_DIR):
	@echo "Création du dossier '$(BUILD_DIR)'..."
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/menus:
	@echo "Création du dossier '$(BUILD_DIR)/menus'..."
	@mkdir -p $(BUILD_DIR)/menus

################################## Archives ###################################

zip: $(ARCHIVES_DATA)
	@echo "Creation de '$(ARCHIVE_NAME).zip'..."
	@zip -r $(ARCHIVE_NAME).zip $(ARCHIVES_DATA) $(SAVES)

targz: $(ARCHIVES_DATA)
	@echo "Creation de '$(ARCHIVE_NAME).tar.gz'..."
	@tar -czvf $(ARCHIVE_NAME).tar.gz $(ARCHIVES_DATA)

#################################### Clean ####################################

cleanall: clean cleansave cleanarch
	

clean:
	@echo "Suppression de l'executable..."
	-@rm -f $(TARGET)
	@echo "Suppression des objets..."
	-@rm -f $(OBJECTS)
cleansave:
	@echo "Suppression des sauvegardes..."
	-@rm -f $(SAVES)

cleanarch:
	@echo "Suppression des archives..."
	-@rm -f $(ARCHIVE_NAME).zip $(ARCHIVE_NAME).tar.gz

###############################################################################

