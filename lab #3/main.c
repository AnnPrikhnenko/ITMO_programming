#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define MAX_STR_LENGTH 500
#define MAX_NUM_OF_SEC 2678400

long long request_time[MAX_NUM_OF_SEC] = { 0 };

typedef struct List {
	time_t value;
	struct List* next;
} List;

/* Queue - FIFO */
void push(List** top, time_t new_value) {
	List* new_list = (List*)malloc(sizeof(List));

	new_list->value = new_value;
	new_list->next = NULL;
	(*top)->next = new_list;
	(*top) = (*top)->next;
}

void pop(List** top) {
	List* prev = NULL;

	prev = (*top);
	(*top) = (*top)->next;
	free(prev);
}

int num_of_month(char* str) {
	if (!strcmp(str, "Jul")) {
		return 6;
	}
	else if (!strcmp(str, "Feb")) {
		return 1;
	}
	else if (!strcmp(str, "Mar")) {
		return 2;
	}
	else if (!strcmp(str, "Apr")) {
		return 3;
	}
	else if (!strcmp(str, "May")) {
		return 4;
	}
	else if (!strcmp(str, "Jun")) {
		return 5;
	}
	else if (!strcmp(str, "Jan")) {
		return 0;
	}
	else if (!strcmp(str, "Aug")) {
		return 7;
	}
	else if (!strcmp(str, "Sep")) {
		return 8;
	}
	else if (!strcmp(str, "Oct")) {
		return 9;
	}
	else if (!strcmp(str, "Nov")) {
		return 10;
	}
	else if (!strcmp(str, "Dec")) {
		return 11;
	}
	else {
		return -1;
	}
}

void pars_time_str(char* str, List** end) {
	char month[MAX_STR_LENGTH];
	struct tm time = { 0 };

	sscanf(str, "%d/%3s/%d:%d:%d:%d",
		&time.tm_mday, month, &time.tm_year,
		&time.tm_hour, &time.tm_min, &time.tm_sec);
	time.tm_mon = num_of_month(month);
	time.tm_isdst = -1;
	time.tm_year -= 1900;

	time_t tmp;
	tmp = mktime(&time);

	push(end, tmp);
}

int get_status(char* str, List** end) {
	char old_str[MAX_STR_LENGTH] = "";
	strcpy(old_str, str);
	char* new_str;

	/* remote_addr */
	new_str = strtok(old_str, "[");

	/* local time */
	new_str = strtok(NULL, "]");
	pars_time_str(new_str, end);

	/* request */
	new_str = strtok(NULL, "\"");
	new_str = strtok(NULL, "\"");

	/* status */
	int status = 0;
	status = atoi(strtok(NULL, " "));
	return status;
}

int main() {
	FILE* fin;
	fin = fopen("access_log_Jul95.txt", "r");
	FILE* fout;
	fout = fopen("error_5xx_requests.txt", "w");
	char cur_str[MAX_STR_LENGTH];

	int num_of_5xx = 0, a = 0;
	int num_of_request = 1;
	int max_num_of_request = -1;
	time_t begin_of_period, end_of_period;

	int period = 0;
	printf("Input period of time: ");
	scanf("%d", &period);
	
	/* queue */
	List* list = (List*)malloc(sizeof(List));
	list->value = 0;
	list->next = NULL;
	List* begin = list;
	List* end = list;

	while (fgets(cur_str, MAX_STR_LENGTH, fin) != NULL) {
		a = get_status(cur_str, &end);
		if (a / 100 == 5) {
			num_of_5xx++;
			fprintf(fout, "¹%i %s", num_of_5xx, cur_str);
		}

		num_of_request++;
		while ((begin->value == 0) || (end->value - begin->value > period)) {
			pop(&begin);
			num_of_request --;
		}
		if (num_of_request > max_num_of_request) {
			max_num_of_request = num_of_request;
			begin_of_period = begin->value;
			end_of_period = end->value;
		}	
	}
	printf("\n\n1. Number of requests with error 5xx - %d\n\n", num_of_5xx);
	printf("2. Period - %d:\nnumber of requests - %d,\ntime:\nfrom %sto %s\n\n\n\n\n", period, max_num_of_request, ctime(&begin_of_period), ctime(&end_of_period));
	
	printf("%lld %lld", begin_of_period, end_of_period);
	fclose(fin);
	fclose(fout);
	return 0;
}
