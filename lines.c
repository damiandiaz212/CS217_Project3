// Damian Diaz (N00821283@students.ncc.edu)
// CSC 217
// Project 3: "Lines"
// Due 12/18/17 11:59 pm
// lines.c

#include "lines.h"

char **allLines;                // Lines from file
char executable_name[MAXCHARS]; // Name of executable

int linesToPrint[MAXARGS]; // Line numbers user requested
int count = 0;             // Amount of lines to print
int amountOfLines;         // Amount of lines found in file

int n = 0;  //-n switch
int f = 0;  //-f switch
int fn = 0; //-fn switch
int nf = 0; //-nf switch

int main(int argc, char *argv[]) {

  FILE *file;

  int numArgs;

  // Determine if file is invoked through stdin or passed through argument
  if (isdigit(*argv[argc - 1]) || argc < 2) {
    file = stdin;
    numArgs = argc;
  } else {
    file = fopen(argv[argc - 1], "r");
    numArgs = argc - 1;
  }

  // store argv[0] in executable_name
  strcpy(executable_name, argv[0]);

  // If file name is invalid or doesnt exist
  if (file == NULL) {
    fprintf(stderr, "%s: %s does not exist\n", executable_name, argv[argc - 1]);
    return 1;
  }

  // If no arguments passed
  if (argc < 2) {
    fprintf(stdout,
            "%s n[n...] [filename]\nDisplay the indicated line(s) of input\n",
            executable_name);
    return 1;
  }

  // Store lines in allLines
  char line[MAXCHARS]; // Current line read (Max 1000 chars)
  int lineNumber = 1;  // Keeps track of line
  int amtLines = 0;
  int c;

  amountOfLines = 0;

  allLines =
      (char **)malloc(amtLines * sizeof(char **)); // Dynamic Memory Allocation

  while (fgets(line, MAXCHARS, file) != NULL) {

    allLines[lineNumber] = (char *)malloc(strlen(line) * MAXCHARS);
    strcpy(allLines[lineNumber], line);
    lineNumber += 1;
    amountOfLines += 1;
  }

  // Store line numbers to print in an array
  for (int i = 1; i < numArgs; i++) {
    addLines(argv[i]);
  }

  // Print linesToPrint
  if (count > 0) {
    printLines();
  }

  // If argument doesnt exist in file
  for (int i = 0; i < count; i++) {
    if (linesToPrint[i] <= 0 || linesToPrint[i] > amountOfLines) {
      fprintf(stderr, "%s: No line %d\n", argv[0], linesToPrint[i]);
    }
  }

  // Close file
  fclose(file);

  return 0;
}

// isDuplicate function
// checks if line number already exists in linesToPrint
int isDuplicate(int num, int lines[]) {

  for (int i = 0; i < count; i++) {
    if (num == lines[i])
      return 1;
  }

  return 0;
}

// printLines function
// prints each linesToPrint that exists in allLines
void printLines() {

  for (int i = 0; i < count; i++) {
    if (linesToPrint[i] > 0 && linesToPrint[i] <= amountOfLines) {

      // Check if user passed any switch arguments
      if (nf == 1)
        printf("%d: %s: %s", linesToPrint[i], executable_name,
               allLines[linesToPrint[i]]);
      else if (fn == 1)
        printf("%s: %d: %s", executable_name, linesToPrint[i],
               allLines[linesToPrint[i]]);
      else if (n == 0 && f == 1)
        printf("%s: %s", executable_name, allLines[linesToPrint[i]]);
      else if (n == 1 && f == 0)
        printf("%d: %s", linesToPrint[i], allLines[linesToPrint[i]]);
      else
        printf("%s", allLines[linesToPrint[i]]);
    }
  }
}

// addLines function
// takes char[] and determines type and value of argument
void addLines(char arg[]) {

  // if arg is a range of integers
  if (strlen(arg) > 1 && isdigit(arg[0]) && strchr(&arg[0], '-')) {

    int a = strtol(&arg[0], &arg, 10); // get min
    int b = strtol(&arg[1], &arg, 10); // get max

    for (int i = a; i <= b; i++) {
      if (isDuplicate(i, linesToPrint) == 0) {
        linesToPrint[count] = i;
        count += 1;
      }
    }

    // if argument is a switch
  } else if (strlen(arg) > 1 && strchr(&arg[0], '-')) {

    if ((strchr(&arg[1], 'f')) && strchr(&arg[2], 'n')) {
      fn = 1;
    } else if ((strchr(&arg[1], 'n') && strchr(&arg[2], 'f'))) {
      nf = 1;
    } else if (strchr(&arg[1], 'n')) {
      n = 1;
    } else if (strchr(&arg[1], 'f')) {
      f = 1;
    }

  }

  // if argument is a single integer
  else {

    int a = strtol(&arg[0], &arg, 10);

    if (isDuplicate(a, linesToPrint) == 0) {
      linesToPrint[count] = a;
      count += 1;
    }
  }
}
