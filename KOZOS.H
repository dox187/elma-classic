

extern double K_pi;
extern double K_pip2;
extern double K_2pi;

#ifdef __APPLE__
#define _fstrnicmp strncasecmp
#elif defined(W_95)
#define _fstrnicmp _strnicmp
#endif

typedef char finame[20];
int fifirst( char* minta, char* filenev );
int finext( char* filenev );
void ficlose( void );
