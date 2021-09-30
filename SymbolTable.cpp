#include "SymbolTable.h"
Node* creativeNode(Node* __NODE , const int &level){
    if(!__NODE) __NODE = new Node(level);
    else {
        Node* p = __NODE;
        __NODE = new Node(level , nullptr , __NODE);
        p->next = __NODE;
    }
    return __NODE;
}
void deleteAllNode(Node* _tail){
    while(_tail){
        Node* p = _tail;
        _tail = _tail->prev;
        if(_tail) _tail->next = NULL;
        delete p;
    }
}
string find_and_cut(string&data ,const string& element){
	string return_string;
	size_t f = data.find(element);
	if(f != string::npos){
		return_string = data.substr(0 , f);
		data.erase(0 , f + 1);
	}
	else{
		return_string = data;
        data.clear();
        return return_string;
	}
	return return_string;
}
bool const_number(const string &number){
    int length = number.length();
    for(int i = 0; i < length; i++){
        if(number[i] < '0' || number[i] > '9') return false;
    }
    return true;
}
bool const_string(const string &string){
    int length = string.length();
    if(string[0] == 39 && string[length - 1] == 39){
        for(int j = 1; j < length - 1 ; ++j){
            if(string[j] == 32) continue;
            else if(string[j] < 48 || (string[j] > 57 && string[j] < 65)
                                    ||(string[j] > 90 && string[j] < 97) || string[j] > 122){
                return false;
            }
        }
    }
    else return false;
    return true;
}
string SymbolTable::INSERT_(Node* pos_node , string &data , const int &level , const string &error) {
    string var = find_and_cut(data , " ");
    string type = data;
    if(type == ""){
        deleteAllNode(pos_node);
        throw InvalidInstruction(error);
    }
    Node* p = pos_node;

    while(p){
        if(p->var == var && p->status){
            deleteAllNode(pos_node);
            throw Redeclared(error);
        }
        if(p->open && p->level == level) break;
        p = p->prev;
    }

    pos_node->var = var;
    pos_node->type = type;
    return "success";
}
string SymbolTable::ASSIGN_(Node* pos_node , string &data , const int &level , const string &error){
    string var = find_and_cut(data , " ");
    string _data_s = data;
    if(_data_s == ""){
        deleteAllNode(pos_node);
        throw InvalidInstruction(error);
    }
    Node * _p_data_s = nullptr;
    bool f_data = 1;
    Node * _pVAR = nullptr;
    bool f_var = 1;
    Node *p = pos_node;
    int _lev = level;
    //bool f_lev = 1;
    while(p){
        if(p->var == var && f_var && p->level == _lev && p->status) {
            _pVAR = p;
            f_var = 0;
        }
        if(p->var == _data_s && f_data && p->level == _lev && p->status) {
            _p_data_s = p;
            f_data = 0;
        }
        if(p->open && p->level == _lev && p->status) --_lev;
        p = p->prev;
    }
    if(_pVAR && _p_data_s){
        if(_pVAR->type == _p_data_s->type){
            _pVAR->data = _p_data_s->data;
            return "success";
        }
        else {
            deleteAllNode(pos_node);
            throw TypeMismatch(error);
        }
    }
    if(!_pVAR || (!_p_data_s && (!const_string(_data_s) && !const_number(_data_s)))) {
        deleteAllNode(pos_node);
        throw Undeclared(error);
    }
    if(const_string(_data_s) && _pVAR->type == "string") {
        _pVAR->data = _data_s;
        return "success";
    }
    else if(const_number(_data_s) && _pVAR->type == "number") {
        _pVAR->data = _data_s;
        return "success";
    }
    else {
        deleteAllNode(pos_node);
        throw TypeMismatch(error);
    }
    return "";

}
void SymbolTable::LOOKUP_(Node* pos_node , const int &lev , const string &var , const string &error){
    Node* Search = pos_node;
    size_t f = var.find(" ");
    if(f != string::npos){
        deleteAllNode(pos_node);
        throw InvalidInstruction(error);
    }
    while(Search){
        if(Search->var == var && Search->level <= lev && Search->status){
            cout << Search->level;
            return;
        }
        else Search = Search->prev;
    }
    deleteAllNode(pos_node);
    throw Undeclared(error);
}
void SymbolTable::PRINT_(Node* pos_node , const int &lev){
    if(!pos_node) return;
    Node* p = pos_node;
    bool f = 1;
    int __LEVEL = lev;
    while(p->prev) p = p->prev;
    Node* __HEAD = p;

    Node* defaut = __HEAD;
    while(defaut){
        if(defaut->seen && defaut->status) defaut->seen = 0;
        defaut = defaut->next;
    }

    Node* head = __HEAD;
    Node* tail = pos_node;

    while(tail->level != 0 && head->level != tail->level){
        while(head){
            if(tail->var == head->var && head->status && tail->status){
                head->seen = 1;
            }
            head = head->next;
            if(head == tail) break;
        }
        head = __HEAD;
        tail = tail->prev;
    }

    p = __HEAD;
    //int __level = __LEVEL;
    int rank = 0;

    while(rank <= __LEVEL){
        while(p){
            if(p->status && p->level == rank && p->var != "" && !p->seen){
                if(f){
                    cout << p->var << "//" << p->level;
                    f = 0;
                    p->print = 1;
                }
                else{
                    cout << " " << p->var << "//" << p->level;
                    p->print = 1;
                }
            }
            p = p->next;
        }
        ++rank;
        p = __HEAD;
    }
    
    p = __HEAD;
    while(p){
        if(p->print && p->status){
            cout << endl;
            break;
        }
        p = p->next;
    }
}
void SymbolTable::RPRINT_(Node* pos_node , const int &lev){
    if(!pos_node) return;
    Node* p = pos_node;
    bool f = 1;
    int __LEVEL = lev;
    while(p->prev) p = p->prev;
    Node* __HEAD = p;

    Node* defaut = __HEAD;
    while(defaut){
        if(defaut->seen && defaut->status) defaut->seen = 0;
        defaut = defaut->next;
    }

    Node* head = __HEAD;
    Node* tail = pos_node;

    while(tail->level != 0 && head->level != tail->level){
        while(head){
            if(tail->var == head->var && head->status && tail->status){
                head->seen = 1;
            }
            head = head->next;
            if(head == tail) break;
        }
        head = __HEAD;
        tail = tail->prev;
    }

    p = pos_node;
    int __level = __LEVEL;

    while(__level >= 0){
        while(p){
            if(p->status && p->level == __level && p->var != "" && !p->seen){
                if(f){
                    cout << p->var << "//" << p->level;
                    f = 0;
                    p->print = 1;
                }
                else{
                    cout << " " << p->var << "//" << p->level;
                    p->print = 1;
                }
            }
            p = p->prev;
        }
        --__level;
        p = pos_node;
    }
    p = __HEAD;
    while(p){
        if(p->print && p->status){
            cout << endl;
            break;
        }
        p = p->next;
    }
}
void SymbolTable::run(string filename)
{
    string line;
	fstream file;

    int __LEVEL = 0;
    Node* __NODE = new Node(__LEVEL);
    //bool flag = 0;
    int countBlock = 0;
    string type;

	file.open(filename , ios::in);
	
	while(!file.eof()){
		getline(file, line);

        string error = line;
        int _size = line.length();
        if(line != "BEGIN" && line != "END" && line != "PRINT" && line != "RPRINT"){
            int _count = 0;
            bool f_ = 0;
            for(int i = 0; i < _size; i++){
                if(!f_){
                    if(line[i] == ' ') _count++;
                    else if(line[i] == 39) f_ = 1;
                }
                else if(f_ && line[i] == 39) f_ = 0;
            }
            if(_count > 2 || line == ""){
                deleteAllNode(__NODE);
                throw InvalidInstruction(error);
            }
        }

        type = find_and_cut(line , " ");
        if(type == "INSERT"){
            __NODE = creativeNode(__NODE , __LEVEL);
            cout << INSERT_(__NODE , line ,__LEVEL, error);
            cout << endl;
        }
        else if (type == "ASSIGN"){
            cout << ASSIGN_(__NODE , line , __LEVEL , error);
            cout << endl;
        }
        else if(type == "BEGIN"){
            if(line != ""){
                deleteAllNode(__NODE);
                throw InvalidInstruction(error);
            }
            ++__LEVEL;
            ++countBlock;
            __NODE = creativeNode(__NODE , __LEVEL);
            __NODE->open = 1;
        }
        else if(type == "END"){
            if(line != ""){
                deleteAllNode(__NODE);
                throw InvalidInstruction(error);
            }
            if(!__NODE){
                throw UnknownBlock();
            }
            --countBlock;
            if(countBlock < 0) {
                deleteAllNode(__NODE);
                throw UnknownBlock();
            }
            Node *p = __NODE;
            while(p){
                if(p) {
                    if(p->open && p->status) {
                        p->closed = 1;
                        p->status = 0;
                        break;
                    }
                    p->status = 0;
                }
                p = p->prev;
            }
            --__LEVEL;
        }
        else if(type == "LOOKUP"){
            LOOKUP_(__NODE ,__LEVEL , line , error);
            cout << endl;
        }
        else if(type == "PRINT"){
            if(line != ""){
                deleteAllNode(__NODE);
                throw InvalidInstruction(error);
            }
            if(__NODE){
                PRINT_(__NODE, __LEVEL);
            }
        }
        else if(type == "RPRINT"){
            if(line != ""){
                deleteAllNode(__NODE);
                throw InvalidInstruction(error);
            }
            if(__NODE){
                RPRINT_(__NODE, __LEVEL);
            }
        }
        else {
            deleteAllNode(__NODE);
            throw InvalidInstruction(error);
        }
	}

	file.close();

    Node * pd = __NODE;
    while(pd) {
        if(pd->open && !pd->closed && pd->status){
            int rank = pd->level;
            deleteAllNode(__NODE);
            throw UnclosedBlock(rank);
        }
        pd = pd->prev;
    }
    deleteAllNode(__NODE);
}