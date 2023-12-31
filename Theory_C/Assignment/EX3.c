#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_STUDENTS 100

typedef struct {
    uint16_t studentId;
    char *name;
    char *gender;
    uint16_t yearOfBirth;
} Student;

typedef struct {
    char *lastName;
    int occurrences;
} LastNameCount;

// Function to calculate the length of a string (without using <string.h>)
char calculateStringLength(const char *str, char *nameLen) {
    char count = 0;
    while (*str != '\0') {
        count++;
        str++;
    }
    if (count > *nameLen) {
        *nameLen = count;
    }
    return *nameLen;
}

// Function to compare two characters (without using <string.h>)
int compareCharacters(char a, char b) {
    if (a < b) {
        return -1;
    }
    else if (a > b) {
        return 1;
    }
    return 0;
}

// Function to compare two names of students alphabetically
int compareNames(const char *name1, const char *name2) {
    while (*name1 && *name2) {
        int result = compareCharacters(*name1, *name2);
        if (result != 0) {
            return result;
        }
        name1++;
        name2++;
    }
    if (*name1) {
        return 1;
    }
    else if (*name2) {
        return -1;
    }
    return 0;
}

// Function to compare two strings (without using <string.h>)
int compareStrings(const char *str1, const char *str2) {
    while (*str1 && *str2) {
        int result = compareCharacters(*str1, *str2);
        if (result != 0) {
            return result;
        }
        str1++;
        str2++;
    }
    if (*str1) {
        return 1;
    }
    else if (*str2) {
        return -1;
    }
    return 0;
}

// Function to extract the last name from the full name
char* extractLastName(const char *fullName) {
    // Get the address of the last character of a string
    while (*fullName != '\0') {
        fullName++;
    }

    // Calculate the number of characters in the last name
    const char *lastSpace = NULL;
    char lastNameLength = 0;
    while (*fullName != ' ') {
        lastSpace = fullName;
        lastNameLength++;
        fullName--;
    }

    if (lastSpace != NULL) {
        // Allocate memory for the string to store the last name
        char *lastName = (char*)malloc((lastNameLength + 1) * sizeof(char));

        // Copy the last name into the string
        for (int i = 0; i < lastNameLength + 1; i++) {
            lastName[i] = lastSpace[i];
        }

        return lastName;
    }

    // If no space is found, return an empty string
    char *emptyString = (char*)malloc(1);
    emptyString[0] = '\0';
    return emptyString;
}

// Function to sort the list of students using the Selection Sort algorithm
void sortStudentsByName(Student arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (compareNames(extractLastName(arr[j].name), extractLastName(arr[minIndex].name)) < 0) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            // Swap
            Student temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}

int main() {
    char fileName[] = "student.csv";
    FILE *file = fopen(fileName, "r");
    Student students[MAX_STUDENTS];
    int numStudents = 0;
    int i;
    char maxNameLength = 0;

    // Read data from the file into the students array
    if (file != NULL) {
        char line[100];
        // Skip the header line
        fgets(line, sizeof(line), file);
        while (fgets(line, sizeof(line), file) != NULL) {
            students[numStudents].name = (char*)malloc(50 * sizeof(char));
            students[numStudents].gender = (char*)malloc(10 * sizeof(char));
            sscanf(line, "%[^,], %[^,], %d, %d",
                students[numStudents].name,
                students[numStudents].gender,
                &students[numStudents].yearOfBirth,
                &students[numStudents].studentId);

            // Calculate the length of the longest name
            maxNameLength = calculateStringLength(students[numStudents].name, &maxNameLength);
            numStudents++;
        }
        fclose(file);
    }
    else {
        printf("Khong the mo file");
        return 1;
    }

    // Enter additional student information
    int numNewStudents;
    printf("Nhap so luong sinh vien can them: ");
    scanf("%d", &numNewStudents);
    for (i = 0; i < numNewStudents; i++) {
        students[numStudents + i].name = (char*)malloc(50 * sizeof(char));
        students[numStudents + i].gender = (char*)malloc(10 * sizeof(char));

        printf("Nhap thong tin sinh vien thu %d:\n", numStudents + i + 1);
        printf("Name: ");
        scanf(" %[^\n]", students[numStudents + i].name);
        printf("Gender: ");
        scanf(" %[^\n]", students[numStudents + i].gender);
        printf("Year of Birth: ");
        scanf("%d", &students[numStudents + i].yearOfBirth);
        printf("Student ID: ");
        scanf("%d", &students[numStudents + i].studentId);

        // Calculate the length of the longest name
        maxNameLength = calculateStringLength(students[numStudents + i].name, &maxNameLength);
    }
    numStudents += numNewStudents;

    // Sort the list of students by name alphabetically
    sortStudentsByName(students, numStudents);

    // Calculate the number of occurrences of each last name
    LastNameCount lastNameCounts[MAX_STUDENTS];
    int numLastNameCounts = 0;

    for (i = 0; i < numStudents; i++) {
        char *currentLastName = extractLastName(students[i].name);

        // Check if the current last name is already counted
        int j;
        for (j = 0; j < numLastNameCounts; j++) {
            if (compareStrings(currentLastName, lastNameCounts[j].lastName) == 0) {
                lastNameCounts[j].occurrences++;
                break;
            }
        }

        // If the last name is not counted yet, add it to the list
        if (j == numLastNameCounts) {
            lastNameCounts[numLastNameCounts].lastName = currentLastName;
            lastNameCounts[numLastNameCounts].occurrences = 1;
            numLastNameCounts++;
        } else {
            // If the current last name is already counted, free the dynamically allocated memory
            free(currentLastName);
        }
    }

    // Write the data back to the file
    file = fopen(fileName, "w");
    if (file != NULL) {
        fprintf(file, "Name, Gender, Year of Birth, Student ID\n");
        for (i = 0; i < numStudents; i++) {
            fprintf(file, "%s, %s, %d, %d\n",
                students[i].name,
                students[i].gender,
                students[i].yearOfBirth,
                students[i].studentId);
        }
        fclose(file);
    }
    else {
        printf("Khong the mo file");
        return 1;
    }

    // Display the sorted list of students
    printf("Danh sach sinh vien:\n");
    printf("NO | %-*s | Gender | Year of Birth | Student ID\n", maxNameLength, "Name");
    for (i = 0; i < numStudents; i++) {
        printf("%-2d | %-*s | %-6s | %-13d | %-10d\n",
            i + 1,
            maxNameLength, students[i].name,
            students[i].gender,
            students[i].yearOfBirth,
            students[i].studentId);
    }

    // Display the occurrences of each last name
    printf("Danh sach ten sinh vien va so lan xuat hien:\n");
    for (i = 0; i < numLastNameCounts; i++) {
        printf("%s: %d\n", lastNameCounts[i].lastName, lastNameCounts[i].occurrences);
    }

    // Free the dynamically allocated memory for students and last names
    for (i = 0; i < numStudents; i++) {
        free(students[i].name);
        free(students[i].gender);
    }

    for (i = 0; i < numLastNameCounts; i++) {
        free(lastNameCounts[i].lastName);
    }

    return 0;
}
