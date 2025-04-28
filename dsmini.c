#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Song {
    char title[100];
    char artist[100];
    int duration; // in seconds
    struct Song* prev;
    struct Song* next;
} Song;

typedef struct Playlist {
    Song* head;
    Song* tail;
    int size;
} Playlist;

Song* createSong(const char* title, const char* artist, int duration) {
    Song* newSong = (Song*)malloc(sizeof(Song));
    if (newSong == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    strncpy(newSong->title, title, sizeof(newSong->title) - 1);
    newSong->title[sizeof(newSong->title) - 1] = '\0';
    strncpy(newSong->artist, artist, sizeof(newSong->artist) - 1);
    newSong->artist[sizeof(newSong->artist) - 1] = '\0';
    newSong->duration = duration;
    newSong->prev = NULL;
    newSong->next = NULL;

    return newSong;
}

void initializePlaylist(Playlist* playlist) {
    playlist->head = NULL;
    playlist->tail = NULL;
    playlist->size = 0;
}

void addSong(Playlist* playlist, const char* title, const char* artist, int duration) {
    Song* newSong = createSong(title, artist, duration);
    if (newSong == NULL) {
        return;
    }

    if (playlist->head == NULL) {
        playlist->head = newSong;
        playlist->tail = newSong;
    } else {
        newSong->prev = playlist->tail;
        playlist->tail->next = newSong;
        playlist->tail = newSong;
    }
    playlist->size++;
    printf("Added '%s' by %s to the playlist.\n", title, artist);
}

void insertSong(Playlist* playlist, int position, const char* title, const char* artist, int duration) {
    if (position < 0 || position > playlist->size) {
        printf("Invalid position!\n");
        return;
    }

    if (position == playlist->size) {
        addSong(playlist, title, artist, duration);
        return;
    }

    Song* newSong = createSong(title, artist, duration);
    if (newSong == NULL) {
        return;
    }

    if (position == 0) {
        newSong->next = playlist->head;
        playlist->head->prev = newSong;
        playlist->head = newSong;
    } else {
        Song* current = playlist->head;
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }
        newSong->next = current->next;
        newSong->prev = current;
        current->next->prev = newSong;
        current->next = newSong;
    }

    playlist->size++;
    printf("Inserted '%s' by %s at position %d.\n", title, artist, position + 1);
}

bool removeSong(Playlist* playlist, const char* title) {
    if (playlist->head == NULL) {
        printf("Playlist is empty!\n");
        return false;
    }

    Song* current = playlist->head;
    while (current != NULL) {
        if (strcmp(current->title, title) == 0) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                playlist->head = current->next;
            }

            if (current->next != NULL) {
                current->next->prev = current->prev;
            } else {
                playlist->tail = current->prev;
            }

            printf("Removed '%s' by %s from the playlist.\n", current->title, current->artist);
            free(current);
            playlist->size--;
            return true;
        }
        current = current->next;
    }

    printf("Song '%s' not found in the playlist.\n", title);
    return false;
}

void displayPlaylist(Playlist* playlist) {
    if (playlist->head == NULL) {
        printf("Playlist is empty!\n");
        return;
    }

    printf("\nCurrent Playlist (%d songs):\n", playlist->size);
    printf("--------------------------------------------------\n");
    printf("No. Title Artist Duration\n");
    printf("--------------------------------------------------\n");

    Song* current = playlist->head;
    int index = 1;
    while (current != NULL) {
        int minutes = current->duration / 60;
        int seconds = current->duration % 60;
        printf("%-4d %-25s %-15s %02d:%02d\n", index, current->title, current->artist, minutes, seconds);
        current = current->next;
        index++;
    }
    printf("--------------------------------------------------\n");
}

void displayPlaylistReverse(Playlist* playlist) {
    if (playlist->tail == NULL) {
        printf("Playlist is empty!\n");
        return;
    }

    printf("\nPlaylist in Reverse Order (%d songs):\n", playlist->size);
    printf("--------------------------------------------------\n");
    printf("No. Title Artist Duration\n");
    printf("--------------------------------------------------\n");

    Song* current = playlist->tail;
    int index = playlist->size;
    while (current != NULL) {
        int minutes = current->duration / 60;
        int seconds = current->duration % 60;
        printf("%-4d %-25s %-15s %02d:%02d\n", index, current->title, current->artist, minutes, seconds);
        current = current->prev;
        index--;
    }
    printf("--------------------------------------------------\n");
}

void searchSong(Playlist* playlist, const char* title) {
    Song* current = playlist->head;
    int position = 1;
    bool found = false;

    while (current != NULL) {
        if (strstr(current->title, title) != NULL) {
            printf("Found at position %d: '%s' by %s (%d seconds)\n", position, current->title, current->artist, current->duration);
            found = true;
        }
        current = current->next;
        position++;
    }

    if (!found) {
        printf("No songs found with '%s' in the title.\n", title);
    }
}

void totalDuration(Playlist* playlist) {
    int total = 0;
    Song* current = playlist->head;

    while (current != NULL) {
        total += current->duration;
        current = current->next;
    }

    int hours = total / 3600;
    int minutes = (total % 3600) / 60;
    int seconds = total % 60;

    printf("Total playlist duration: %02d:%02d:%02d\n", hours, minutes, seconds);
}

void freePlaylist(Playlist* playlist) {
    Song* current = playlist->head;
    while (current != NULL) {
        Song* temp = current;
        current = current->next;
        free(temp);
    }

    playlist->head = NULL;
    playlist->tail = NULL;
    playlist->size = 0;
}

void displayMenu() {
    printf("\nMusic Playlist Organizer\n");
    printf("1. Add a song to the end\n");
    printf("2. Insert a song at position\n");
    printf("3. Remove a song by title\n");
    printf("4. Display playlist\n");
    printf("5. Display playlist in reverse\n");
    printf("6. Search for a song\n");
    printf("7. Show total duration\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    Playlist myPlaylist;
    initializePlaylist(&myPlaylist);

    int choice;
    char title[100];
    char artist[100];
    int duration;
    int position;

    // Add some initial songs
    addSong(&myPlaylist, "Bohemian Rhapsody", "Queen", 354);
    addSong(&myPlaylist, "Hotel California", "Eagles", 390);
    addSong(&myPlaylist, "Imagine", "John Lennon", 183);

    while (1) {
        displayMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Exiting.\n");
            break;
        }
        getchar(); // consume newline

        switch (choice) {
            case 1:
                printf("Enter song title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0';

                printf("Enter artist: ");
                fgets(artist, sizeof(artist), stdin);
                artist[strcspn(artist, "\n")] = '\0';

                printf("Enter duration in seconds: ");
                scanf("%d", &duration);
                getchar();

                addSong(&myPlaylist, title, artist, duration);
                break;

            case 2:
                printf("Enter position (1-%d): ", myPlaylist.size + 1);
                scanf("%d", &position);
                getchar();

                printf("Enter song title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0';

                printf("Enter artist: ");
                fgets(artist, sizeof(artist), stdin);
                artist[strcspn(artist, "\n")] = '\0';

                printf("Enter duration in seconds: ");
                scanf("%d", &duration);
                getchar();

                insertSong(&myPlaylist, position - 1, title, artist, duration);
                break;

            case 3:
                printf("Enter title of the song to remove: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0';

                removeSong(&myPlaylist, title);
                break;

            case 4:
                displayPlaylist(&myPlaylist);
                break;

            case 5:
                displayPlaylistReverse(&myPlaylist);
                break;

            case 6:
                printf("Enter part of the title to search: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0';

                searchSong(&myPlaylist, title);
                break;

            case 7:
                totalDuration(&myPlaylist);
                break;

            case 8:
                freePlaylist(&myPlaylist);
                printf("Exiting program.\n");
                return 0;

            default:
                printf("Invalid choice! Please try again.\n");
                break;
        }
    }

    freePlaylist(&myPlaylist);
    return 0;
}
