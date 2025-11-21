#ifndef DATABASE_H
#define DATABASE_H

struct database {
    int   (*init)           ( void );
    int   (*add)            ( char *name, int word_count, char *author );
    int   (*get_word_count) ( int doc_id );
    char* (*get_name)       ( int doc_id );
    void  (*stat)           ( void );
    void  (*quit)           ( void );
};

void database_bind_listdb ( struct database *db );
void database_bind_bstdb  ( struct database *db );

#endif
