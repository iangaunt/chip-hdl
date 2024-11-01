#ifndef DFF_H
#define DFF_H

#include <vector>

using std::vector;

class dff {
    public:
        static vector<dff> v;
        static int next_index;
        
        dff();
        static void update_dffs();
        
        void update(bool in);
    
        int dff_index;
        bool out;
        bool curr;
};

#endif 