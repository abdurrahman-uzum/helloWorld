/*
	EED1010 / EXAMPLE #07 / TASK #01

	Abdurrahman ÜZÜM

	Date: 08.04.2020


	Function: Accpets account number, name and balance inputs from user and prints to a text file, displays the content. 

	Program pulls the data as string, processes it as string and writes as string.
	However, it also checks if user is entering correct types and displays an error
	message in case of unexpected entry and ignores the related entry.

	Note: I used strtok() for this task and sscanf() for the second task for parsing strings. This is mainly because 
	I didn't know about sscanf() while writing this... And it now occurs to me that I probably could have
	done both of them with regular scanf()/fscanf() but in the end scanf isn't the securest function family anyway...

*/



#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



//Global file pointer:
FILE* output = NULL;




/*
	Third condition in the if statement enables user to enter
	letters which does not exist in the English alphabet.
*/
int isString( char* string )
{
	for ( int i=0; string[i+1] != '\0'; i++ )
	{
		if ( !(  (string[i] >= 'A' && string[i] <= 'Z')  ||  (string[i] >= 'a' && string[i] <= 'z')  ||  (string[i] >= 'Ç' && string[i] <= 'Ñ')  ) )
		{
			return 0;
		}
	}

	return 1;
}





int isNumber( char* string )
{
	for ( int i=0; string[i+1] != '\0'; i++ )
	{
		if ( !(  (string[i] >= '0' && string[i] <= '9')  ||  string[i] == '.'   ) )
		{
			return 0;
		}
	}

	return 1;
}





/*
	Function: parseInput

	Parses given string, namely buffer, into three char arrays
	according to whitespaces in the main string, buffer.

	Checks whether parsed sub-parts are as intended.
	If not, returns negative of the sub-part number as error code.

	Returns the number of sub-parts othervise.

	Return value will be passed to handleError function.
*/

int parseInput( char* buffer, char* accnum, char* name, char* balance )
{
	int parsed = 0;
	char* pointer = NULL;


	pointer = strtok( buffer, " " );

	if ( pointer == NULL || !isNumber( pointer ) )
	{
		return -(++parsed);
	}
	else
	{
		strcpy( accnum, pointer );
		parsed++;
	}



	pointer = strtok( NULL, " " );

	if ( pointer == NULL || !isString( pointer ) )
	{
		return -(++parsed);
	}
	else
	{
		strcpy( name, pointer );
		parsed++;
	}



	pointer = strtok( NULL, "\n" );

	if ( pointer == NULL || !isNumber( pointer ) )
	{
		return -(++parsed);
	}
	else
	{
		strcpy( balance, pointer );
		parsed++;
	}



	return parsed;

}




/*
	Function: handleError

	Accepts the code returned by parseInput function.
	If code corresponds to an error, accordingly prints the necessary information to console.
	Does nothing otherwise.

	Return value of this function will be checked in the main function
	and if it's smaller than three, input will not be printed to the file.
*/

int handleError( int code )
{
	if ( code >= 0 && code < 3 )
	{
		printf("Entry ignored: missing information.\n");
		printf(">");
	}


	switch ( code )
	{
		case -1:
			printf("Entry ignored: unexpected character at input: 1\n");
			printf(">");
		break;

		case -2:
			printf("Entry ignored: unexpected character at input: 2\n");
			printf(">");
		break;

		case -3:
			printf("Entry ignored: unexpected character at input 3\n");
			printf(">");
		break;
	}

	return (code < 3);
}





void writeFile( char* accnum, char* name, char* balance )
{
	//Print titles.
	if ( accnum == NULL && name == NULL && balance == NULL )
	{
		fprintf( output, "%20s %20s %20s\n", "Account Number", "Name", "Balance" );

		return;
	}

	//Print passed data.
	fprintf( output, "%20s %20s %20s\n", accnum, name, balance );
}






void printFile()
{
	char c;

	fseek( output, 0, SEEK_SET );

	while ( !feof(output) )
	{
		c = fgetc(output);
		printf("%c", c);
	}
}





int main ( void )
{
	// Initialize //

	output = fopen( "output.txt", "w" );

	if ( output == NULL )
	{
		printf("Fatal Error: Failed to create / open file. \n");
		exit(-1);
	}

	//Empty call for printing titles:
	writeFile( NULL, NULL, NULL );

	//Temporary holders:
	char buffer[300] = { 0 };
	char accnum[100] = { 0 };
	char name[100] = { 0 };
	char balance[100] = { 0 };

	printf("Enter account number, name and balance values. Enter EOF terminate input process.\n\n");
	printf(">");



	// Writing to file //

	while ( fgets(buffer, 300, stdin) )
	{
		printf(">");
		int code = 0;

		code = parseInput(buffer, accnum, name, balance);

		// In the case of any errors, do not print to the file.
		if ( handleError( code ) )
		{
			continue;
		}


		writeFile( accnum, name, balance );
	}
	fclose ( output );




	// Reading file //

	printf( "File content:\n");
	
	output = fopen( "output.txt", "r" );
	
	printFile();



	
	// Terminate //

	fclose ( output );

	return 0;
}