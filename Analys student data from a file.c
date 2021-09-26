#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>


/*Structurs declaration*/
typedef struct Student {
	char *name;
	long ID;
	int test_grade;
	char grades[5];
	char flag;
	char final_grade;
}Student;

typedef struct University {
	Student* arr;
	int num_students;
}University;

void input_data(FILE *f, University* univ);
void output_data(FILE *f, University* univ);
void output_data_finalgrade(FILE* f, University* univ);
void statistic_data(FILE* f, University* univ, char prog_check);
void black_list(FILE* f, University* univ);
void exit_free(University* univ);
int Bit_Count(char grades[]);


void main() {
	int action_num = 0, i;
	char prog_check; /*This variable contains the letter that indicates whether the student is in the new / old program*/
	FILE *in, *out;
	University univ;
	printf("Please enter your number of action:\n");
	printf("1->input data, 2->output data, 3->final grade calculate, 4->statistic data, 5->blacklist, 6->end of program\n");
	scanf("%d", &action_num);
	while (action_num != 1) { /*The loop "forces" the user to select the number 1 (information reading) as the first choice*/
		printf("Your first choice must be 1 for input data!\n");
		printf("Please enter your number of action:\n");
		printf("1->input data, 2->output data, 3->final grade calculate, 4->statistic data, 5->blacklist, 6->end of program\n");
		scanf("%d", &action_num);
	}
	in = fopen("input.txt", "r");
	if (in == NULL) exit(1); /*Checks if the file open properly*/
	input_data(in, &univ);
	fclose(in); /*Once we are done working with the input file we will start working with the output file*/
	out = fopen("output.txt", "w");
	if (out == NULL) exit_free(&univ);
	while (action_num != 6) { /*The loop checks which number (action) the user selects and calls the relevant function*/
		printf("Please enter your number of action:\n");
		printf("1->input data, 2->output data, 3->final grade calculate, 4->statistic data, 5->blacklist, 6->end of program\n");
		scanf("%d", &action_num);
		if (action_num == 1) {
			printf("You already picked 1!\n");
		}
		else if (action_num == 2) {
			fprintf(out, "\nOption 2\n");
			output_data(out, &univ);
		}
		else if (action_num == 3) {
			fprintf(out, "\nOption 3\n");
			output_data_finalgrade(out, &univ);
		}
		else if (action_num == 4) {
			fprintf(out, "\nOption 4\n");
			printf("Type Y for new study program, N for old study program:\n");
			rewind(stdin);
			prog_check = getchar();
			while (prog_check != 'Y' && prog_check != 'N') {
				printf("Type Y for new study program, N for old study program:\n");
				rewind(stdin);
				prog_check = getchar();
			}
			statistic_data(out, &univ, prog_check);
		}
		else if (action_num == 5) {
			fprintf(out, "\nOption 5\n");
			black_list(out, &univ);
		}
		else if (action_num != 6) {
			printf("Pick number in range(2-6)\n");
			printf("Please enter your number of action:\n");
			printf("1->input data, 2->output data, 3->final grade calculate, 4->statistic data, 5->blacklist, 6->end of program\n");
			scanf("%d", &action_num);
		}
		
	}
	fprintf(out, "\nOption6\nEnd of Program");
	fclose(out);
	for (i = 0; i < univ.num_students; i++) { /*When the program is over we will release all the memory allocations*/
		free(univ.arr[i].name);
	}
	free(univ.arr);
}

/*The function gets pointer to file and pointer to university,
input the students data from file to students array in university, and count the number od students*/
void input_data(FILE* f, University* univ) {
	char temp_name[100], temp_c; /*Using a temp string for the name will help us know the length of the name,
 so we can copy it to a name variable in a student structure efficiently*/
	int i = 0, j;
	univ->arr = (Student*)malloc(sizeof(Student));  //Allocation memory for students array
	if (univ->arr == NULL) exit(1);
	while (fscanf(f, "%s", temp_name) != -1) {
		univ->arr = (Student*)realloc(univ->arr, (i+1)*sizeof(Student)); //Reallocation memory for students array
		if (univ->arr == NULL) exit_free(univ);
		univ->arr[i].name = (char*)malloc((strlen(temp_name)+1)*sizeof(char)); //Allocation memory for student name
		if (univ->arr[i].name == NULL) exit_free(univ);
		strcpy(univ->arr[i].name, temp_name);
		fscanf(f, "%ld", &univ->arr[i].ID);
		fscanf(f, "%d", &univ->arr[i].test_grade);
		fscanf(f, "%c", &temp_c); /*skip space*/
		for (j = 0; j < 5; j++) {
			fscanf(f, "%c", &univ->arr[i].grades[j]);
		}
		fscanf(f, "%c", &temp_c);
		fscanf(f, "%c", &univ->arr[i].flag);
		univ->arr[i].final_grade = 0;
		if (Bit_Count(univ->arr[i].grades) >= 3)
			univ->arr[i].final_grade = 1;
		i++;
	}
	univ->num_students = (i);
}

/*The function gets pointer to file and pointer to university,
print the data of students from array in univ to the file*/
void output_data(FILE* f, University* univ) {
	int i;
	for (i = 0; i < univ->num_students; i++) {
		fprintf(f, "Student %d: %s %ld %d %d %c\n", i + 1, univ->arr[i].name, univ->arr[i].ID, univ->arr[i].test_grade, univ->arr[i].final_grade, univ->arr[i].flag);
	}
}

/*The function gets pointer to file and pointer to university,
print the data of students from array in univ to the file, including their final grade*/
void output_data_finalgrade(FILE* f, University* univ) {
	int i;
	int temp_finalgrade;
	fprintf(f, "BEFORE:\n");
	output_data(f,univ);
	fprintf(f, "\nAFTER:\n");
	for (i = 0; i < univ->num_students; i++) {
		temp_finalgrade = univ->arr[i].test_grade;
		if (univ->arr[i].test_grade >= 55 && univ->arr[i].final_grade == 1) {
			if (univ->arr[i].flag == 'Y')
				temp_finalgrade = (univ->arr[i].test_grade * 0.8) + (100 * 0.2);
			else
				temp_finalgrade = (univ->arr[i].test_grade * 0.9) + (100 * 0.1);
		}
		fprintf(f, "Student %d: %s %ld %d %d final: %d\n", i + 1, univ->arr[i].name, univ->arr[i].ID, univ->arr[i].test_grade, univ->arr[i].final_grade, temp_finalgrade);
	}
}

/*The function gets pointer to file and pointer to university,
calculate statistic data of test for studnets in new/old progrem and print them in the file*/
void statistic_data(FILE* f, University* univ, char prog_check) {
	int i, sum = 0, min_grade = 100, max_grade = 0, num_students_new = 0, num_students_old;
	float average, standard_deviation;
	for (i = 0; i < univ->num_students; i++) { //calcuate num of students in new/old program
		if (univ->arr[i].flag == 'Y')
			num_students_new++;
	}
	num_students_old = univ->num_students - num_students_new;
	for (i = 0; i < univ->num_students; i++) { //calcuate sum of test grades
		if (univ->arr[i].flag == prog_check) {
			if (univ->arr[i].test_grade > max_grade)
				max_grade = univ->arr[i].test_grade;
			if (univ->arr[i].test_grade < min_grade)
				min_grade = univ->arr[i].test_grade;
			sum = sum + univ->arr[i].test_grade;
		}
	}
	if (prog_check == 'Y')
		average = (float)sum / (float)num_students_new;
	else
		average = (float)sum / (float)num_students_old;
	sum = 0;
	for (i = 0; i < univ->num_students; i++) { //calcuate standard deviation of test grades
		if (univ->arr[i].flag == prog_check)
			sum = sum + pow((univ->arr[i].test_grade- average) , 2);
	}
	if (prog_check == 'Y')
		standard_deviation = sqrt((float)sum / (num_students_new));
	else
		standard_deviation = sqrt((float)sum / num_students_old);
	fprintf(f, "1.(average,x = %.2f)\n2.(standrad divition,Sd = %.2f)\n3.(num of students, %d)\n4.(Range = %d - %d)\n", average, standard_deviation, univ->num_students, max_grade, min_grade);
	if (prog_check == 'Y')
		fprintf(f, "5.(%d students in this program)\n\n",num_students_new);
	else
		fprintf(f, "5.(%d students in this program)\n\n",num_students_old);
}

/*The function gets pointer to file and pointer to university,
and print the blacklist (Students who do not meet the conditions)*/
void black_list(FILE* f, University* univ) {
	int i;
	fprintf(f, "The blacklist is:\n");
	for (i = 0; i < univ->num_students; i++) { //calculate num of students in new/old program
		if (univ->arr[i].final_grade == 0)
			fprintf(f, "name: %s, ID: %ld\n", univ->arr[i].name, univ->arr[i].ID);
	}
}


//The function gets array of chars and reutrn how many '1' in array
int Bit_Count(char grades[]) {
	int i, count = 0;
	for (i = 0; i < 5; i++) {
		if ((grades[i] - '0') == 1) /*Subtract the ascii value of 0 to get an numeric value*/
			count++;
	}
	return count;
}

//The function gets pointer to University, free all memory and exit program 
void exit_free(University* univ) {
	int i;
	for (i = 0; i < univ->num_students; i++) {
		free(univ->arr[i].name);
	}
	free(univ->arr);
	exit(1);
}
