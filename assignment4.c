#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct instruction_s
{
	int step;
	int index;
};

char* get_word(char* s, int n);
void get_sentence(char** lines, struct instruction_s* instructions, int n_instructions, char* sentence);

int main(int argc, char* argv[])
{
	FILE* alice;
	FILE* inst;
	char** book; // representing book
	struct instruction_s* instructions; // representing the instructions
	int n_instructions = 0;
	int i, line = 10000, characters_in_line = 200;
	instructions = malloc(line * sizeof(struct instruction_s));
	book = malloc(line * sizeof(char*));

	for (i = 0; i < line; ++i)
	{
		book[i] = malloc(characters_in_line * sizeof(char));
	}

	i = 0;
	if ((alice = fopen(argv[1], "r")))
	{
		while ((fgets(book[i], 200, alice)) != NULL)
		{
			// printf("%s", book[i]);
			i++;
		}
	}
	fclose(alice);

	i = 0;
	if ((inst = fopen(argv[2], "r")))
	{
		while (fscanf(inst, "%d %d", &instructions[i].step, &instructions[i].index) != EOF)
		{
			// printf("%d %d\n", instructions[i].step, instructions[i].index);
			++i;
		}
	}
	n_instructions = i;
	fclose(inst);

	char* sent = malloc(n_instructions * 200 * sizeof(char));
	get_sentence(book, instructions, n_instructions, sent);
	printf("%s", sent);
	
	free(sent);
	free(instructions);
	for(int i = 0; i < line; i++)
		free(book[i]);
	free(book);
	return 0;
}

char* get_word(char* s, int n)
{
	int len = strlen(s);
	char* fixed = malloc(len * sizeof(char));
	int k = 0;
	for (int i = 0; i < len; ++i)
	{
		if (s[i] >= 'A' && s[i] <= 'Z')
			fixed[k++] = tolower(s[i]);
		else if ((s[i] >= 'a' && s[i] <= 'z') || s[i] == ' ')
			fixed[k++] = s[i];
	}
	fixed[k] = '\0';

	char* ret = malloc(len * sizeof(char));
	int word = 0;
	int word_start = 0;
	for (int i = 0; i < len && word <= n; i++)
	{
		if (fixed[i] != ' ' && (i == 0 || fixed[i - 1] == ' '))
		{
			word++;
			word_start = i;
		}

		if (word == n && fixed[i] != ' ')
		{
			ret[i - word_start] = fixed[i];
			ret[i - word_start + 1] = '\0';
		}
	}

	free(fixed);
	return ret;
}

void get_sentence(char** lines, struct instruction_s* instructions, int n_instructions, char* sentence)
{
	int sent_inc = 0;
	int inc = 0;

	for (int i = 0; i < n_instructions; i++)
	{
		inc += instructions[i].step;
		char* word = get_word(lines[inc - 1], instructions[i].index);
		for (int j = 0; j < strlen(word); j++)
		{
			sentence[sent_inc++] = word[j];
		}
		if (sentence[sent_inc] != ' ')
			sentence[sent_inc++] = ' ';
		free(word);
	}
	sentence[sent_inc++] = '\0';
}