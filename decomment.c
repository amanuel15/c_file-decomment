#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int mainHandler();

enum status{
    none,

    maybe_com,
    openComment,
    maybeClose_com,

    charLiteral,
    stringLiteral,

    escapedChar,
    escapedString
};
/*------------------------------------------------------------*/
/* handleNoneStatus: Implement the none state of the DFA.      */
/* c is the current DFA character.  Return the next state.    */
/*------------------------------------------------------------*/
enum status handleNoneStatus(char c){
    enum status status1;
    if(c == '/'){
        status1 = maybe_com;
    } else if (c == '\''){
        putchar(c);
        status1 = charLiteral;
    } else if (c == '"'){
        putchar(c);
        status1 = stringLiteral;
    } else{
        putchar(c);
        status1 = none;
    }
    return status1;
}
/*-------------------------------------------------------------*/
/* handleMaybe_comStatus: Implement the maybe_com state of the */
/* DFA. c is the current DFA character.  Return the next state.*/
/*-------------------------------------------------------------*/
enum status handleMaybe_comStatus(char c){
    enum status status1;
    if(c == '*'){
        status1 =  openComment;
    } else if(c =='/'){
        putchar('/');
        status1 = maybe_com;
    } else{
        putchar('/');
        putchar(c);
        status1 = none;
    }
    return status1;
}
/*-----------------------------------------------------------------*/
/* handleOpenCommentStatus: Implement the openComment state of     */
/* the DFA c. is the current DFA character.  Return the next state.*/
/*-----------------------------------------------------------------*/
enum status handleOpenCommentStatus(char c){
    enum status status1;
    if(c == '*'){
        status1 =  maybeClose_com;
    } else{
        status1 = openComment;
    }
    return status1;
}
/*-----------------------------------------------------------------*/
/* handleMaybeClose_comStatus: Implement the maybeClose state of   */
/* the DFA. c is the current DFA character.  Return the next state.*/
/*-----------------------------------------------------------------*/
enum status handleMaybeClose_comStatus(char c){
    enum status status1;
    if(c == '/'){
        status1 =  none;
    } else if(c == '*'){
        status1 = maybeClose_com;
    } else{
        status1 = openComment;
    }
    return status1;
}
/*-----------------------------------------------------------------*/
/* handleCharLiteralStatus: Implement the charLiteral state of     */
/* the DFA. c is the current DFA character.  Return the next state.*/
/*-----------------------------------------------------------------*/
enum status handleCharLiteralStatus(char c){
    enum status status1;
    if(c == '\''){
        putchar(c);
        status1 =  none;
    } else if(c == '\\'){
        status1 = escapedChar;
    } else{
        putchar(c);
        status1 = charLiteral;
    }
    return status1;
}
/*-----------------------------------------------------------------*/
/* handleStringLiteralStatus: Implement the stringLiteral state of */
/* the DFA. c is the current DFA character.  Return the next state.*/
/*-----------------------------------------------------------------*/
enum status handleStringLiteralStatus(char c){
    enum status status1;
    if(c == '"'){
        putchar(c);
        status1 = none;
    } else if(c == '\\'){
        status1 = escapedString;
    } else{
        putchar(c);
        status1 = stringLiteral;
    }
    return status1;
}
/*-----------------------------------------------------------------*/
/* handleEscapedCharStatus: Implement the escapeChar state of the  */
/* DFA. c is the current DFA character.  Return the next state.    */
/*-----------------------------------------------------------------*/
enum status handleEscapedCharStatus(char c){
    enum status status1 = charLiteral;
    putchar(c);
    return status1;
}
/*-----------------------------------------------------------------*/
/* handleEscapedStringStatus: Implement the escapeString state of  */
/* the DFA. c is the current DFA character.  Return the next state.*/
/*-----------------------------------------------------------------*/
enum status handleEscapedStringStatus(char c){
    enum status status1 = stringLiteral;
    putchar(c);
    return status1;
}

/*------------------------------------------------------------*/
/* main: Calls the mainHandler method to do the whole thing.  */
/* Return 0 if successful or EXIT_FAILURE if not.                                                  */
/*------------------------------------------------------------*/
int main(void) {

    int EXIT_FAIL = mainHandler();

    if(EXIT_FAIL) return EXIT_FAILURE;
    return 0;
}
/*------------------------------------------------------------*/
/*MainHandler: Read text from stdin. De-comment the whole     */
/*file given to it from the input re-director where the input */
/*can be any text format including '*.c'. Write the result to */
/*stdout. Returns void.                                       */
/*------------------------------------------------------------*/
int mainHandler(){

    int c, line = 0, commentLine = 0; /*The two variables line and commentLine are line counters*/
    enum status stat = none;

    do {
        c = getchar();
        if(c == EOF) break;
        /*This two ifs are used to determine lines of unterminated comments*/
        if(c == '\n' & stat != openComment){ line+=(1+commentLine); commentLine=0;}
        if(stat == openComment & c=='\n') commentLine +=1;
        switch (stat){
            case none:
                stat = handleNoneStatus(c);
                break;
            case maybe_com:
                stat = handleMaybe_comStatus(c);
                break;
            case openComment:
                stat = handleOpenCommentStatus(c);
                break;
            case maybeClose_com:
                stat = handleMaybeClose_comStatus(c);
                break;
            case charLiteral:
                stat = handleCharLiteralStatus(c);
                break;
            case stringLiteral:
                stat = handleStringLiteralStatus(c);
                break;
            case escapedChar:
                stat = handleEscapedCharStatus(c);
                break;
            case escapedString:
                stat = handleEscapedStringStatus(c);
                break;
        }
    }
    while(c != EOF);
    /*Prints the success message onto the terminal.*/
    if(stat == openComment){
        fprintf(stderr, "Error: line %d: unterminated comment.\n", line+1);
        return 1;
    } else{
        fprintf(stderr,"SUCCESSFUL!!!.\n");
        return 0;
    }

}