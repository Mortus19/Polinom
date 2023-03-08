#include <iostream>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

constexpr int l = -10;
constexpr int r = 10;
constexpr int p = r-l+1;

vector<int> decoding(int x){
    vector<int>ans(3);
    for(int i = 0;i<3;i++){
        ans[2-i] = (x%p)+l;
        x = x/p;
    }
    return ans;
}

int encoding(int x, int y , int z){
    int ans = 0;int st = 1;
    x-=l;y-=l;z-=l;
    ans+=st * z;
    st*=p;
    ans+=st*y;
    st*=p;
    ans+=st*x;
    return ans;
}
int get_X(int pow){
    return (pow/(p*p))%p + l;
}
int get_Y(int pow){
    return (pow/p)%p + l;
}
int get_Z(int pow){
    return (pow%p)+l;
}

double binpow(double a, int x){
    if(x<0){
        return (1.0/binpow(a,-x));
    }
    if(x == 0)
        return 1;
    if(x%2 ==1)
        return a* binpow(a,x-1);
    double ans = binpow(a,x/2);
    return ans*ans;
}

struct Monom{
    int pow;//закодированны 3 числа в p-ичной системе счисления
    double c;//коэффицент
    Monom(int x = l, int y = l , int z = l, double _c = 0){
        pow = encoding(x,y,z);
        c = _c;
    }
    Monom(const string& s){
        int pow_x = 0;int pow_y = 0;int pow_z = 0;
        string t = "";
        int last_st = 0;
        //x ->last_st = 1,
        //y ->last_st = 2,
        //z ->last_st = 3
        c = 0;
        for(int i = 0;i<s.size();i++){
            if(s[i] == '^'){
                t = "";
                continue;
            }
            else if(s[i] !='x' && s[i] !='y' && s[i]!='z') {
                t+=s[i];
            }
            else{
                if(s[i] == 'x') {
                    if(t!="") {
                        c = stod(t);
                    }
                    t = "";
                    pow_x = 1;
                    last_st = 1;
                }
                else if(s[i] == 'y') {
                    if(t != ""){
                        if(last_st == 0) {
                            c = stod(t);
                        }
                        if(last_st == 1) {
                            pow_x = stoi(t);
                        }
                    }
                    pow_y = 1;
                    last_st = 2;
                }
                else {
                    if(t != ""){
                        if(last_st == 0) {
                            c = stod(t);
                        }
                        if(last_st == 1) {
                            pow_x = stoi(t);
                        }
                        if(last_st == 2) {
                            pow_y = stoi(t);
                        }
                    }
                    pow_z = 1;
                    last_st = 3;
                }
                t = "";
            }
        }
        if(t != ""){
            if(last_st == 0) {
                c = stod(t);
            }
            if(last_st == 1) {
                pow_x = stoi(t);
            }
            if(last_st == 2) {
                pow_y = stoi(t);
            }
            if(last_st == 3) {
                pow_z = stoi(t);
            }
        }
        pow = encoding(pow_x,pow_y,pow_z);
        if(c == 0)
            c = 1;
    }
    Monom(const Monom& obj){
        pow = obj.pow;
        c = obj.c;
    }
    Monom& operator = (const Monom& obj){
        pow = obj.pow;
        c = obj.c;
        return *this;
    }
    Monom operator *(Monom& obj){
        Monom ans;
        ans.c = c*obj.c;
        ans.pow = encoding(get_X(pow)+get_X(obj.pow),get_Y(pow)+get_Y(obj.pow),get_Z(pow)+get_Z(obj.pow));
        if(get_X(pow)+get_X(obj.pow) > r || get_X(pow)+get_X(obj.pow) < l)
            ans.pow = -1;
        if(get_Y(pow)+get_Y(obj.pow) > r || get_Y(pow)+get_Y(obj.pow) < l)
            ans.pow = -1;
        if(get_Z(pow)+get_Z(obj.pow) > r || get_Z(pow)+get_Z(obj.pow) < l)
            ans.pow = -1;
        return ans;
    }
    double calc(double x, double y , double z){
        double ans = c;
        ans *= binpow(x,get_X(pow));
        ans *= binpow(y,get_Y(pow));
        ans *= binpow(z,get_Z(pow));
        return ans;
    }
    bool operator > (const Monom& obj){
        return pow > obj.pow;
    }
    bool operator >= (const Monom& obj){
        return pow >= obj.pow;
    }
    bool operator < (const Monom& obj){
        return pow < obj.pow;
    }
    bool operator <= (const Monom& obj){
        return pow <= obj.pow;
    }
    bool operator == (const Monom& obj){
        return (pow == obj.pow);
    }
    bool operator != (const Monom& obj) {
        if(*this==obj)
            return false;
        return true;
    }
    friend ostream& operator<<(ostream& out ,Monom& m){
        int t;
        if (m.c > 0.0) out << "+";
        out << m.c;
        t = get_X(m.pow); if (t != 0) out << "x^" << t;
        t = get_Y(m.pow); if (t != 0) out << "y^" << t;
        t = get_Z(m.pow); if (t != 0) out << "z^" << t;
        return out;
    }
};

struct Link{
    Link* next;
    Monom m;
    Link(Monom x){
        m = x;
        next = nullptr;
    }
};

class Polinom {
    //Список, элементы которого отсортированы по возврастанию
    Link* start;
    Link* end;
public:
    Polinom() {
        start = nullptr;
        end = nullptr;
    }
    Polinom(const Polinom& obj){
        start = nullptr;
        end = nullptr;
        Link* t = obj.start;
        while(t!= nullptr){
            this->add(t->m);
            t = t->next;
        }
    }
    ~Polinom(){
        this->clear();
    }
    void add(Monom x){
        //считаем , что список упорядочен
        Link* t = start;
        Link* tmp = new Link(x);
        if(start == nullptr){
            start = tmp;
            end = tmp;
            return;
        }
        else if(end->m.pow < x.pow){
            //Если наш элемент больше всякого в листе, мы его добавляем в конец
            //и выходим
            end->next = tmp;
            end = tmp;
            return;
        }
        else if(end->m.pow == x.pow){
            end->m.c += tmp->m.c;
            if(end->m.c == 0){
                this->erase(end->m);
            }
            return;
        }
        else if(start->m.pow > x.pow){
            //Если наш элемент меньше всякого в листе, мы его добавляем в начало
            //и выходим
            tmp->next = start;
            start = tmp;
            return;
        }
        else if(start->m.pow == x.pow){
            start->m.c += tmp->m.c;
            if(start->m.c == 0){
                this->erase(start->m);
            }
            return;
        }
        while(t->next != nullptr){
            if(t->next->m.pow > x.pow){
                //добавляем
                tmp->next = t->next;
                t->next = tmp;
                return;
            }
            if(t->next->m.pow == x.pow){
                t->next->m.c += x.c;
                if(t->next->m.c == 0){
                    this->erase(t->next->m);
                }
                return;
            }
            t = t->next;
        }
    }
    void add(int x, int y, int z, int c){
        Monom m(x,y,z,c);
        this->add(m);
    }
    void erase(Monom x){
        //Удаляет этот моном из листа
        if(this->find(x) == false){
            throw exception();
        }
        if(start == end){
            //1 элемент в листе и его мы удаляем
            delete start;
            start = nullptr;
            end = nullptr;
            return;
        }
        Link* t = start;
        while(t->next!=end){
            if(t->next->m == x){
                Link* tmp = t->next;
                t = tmp->next;
                delete tmp;
                return;
            }
            t = t->next;
        }
        delete end;
        end = t;
        end->next = nullptr;
    }
    void erase(int x,int y, int z, int c){
        Monom m(x,y,z,c);
        this->erase(m);
    }
    double calc(double x , double y , double z){
        double ans = 0;
        Link* t = start;
        while(t!=nullptr){
            ans+= t->m.calc(x,y,z);
            t = t->next;
        }
        return ans;
    }
    bool find(Monom x){
        Link* t =start;
        while(t != nullptr){
            if(t->m == x)
                return true;
            t = t->next;
        }
        return false;
    }
    void clear(){
        Link* t = start;
        Link* sled;
        while(t!= nullptr){
            sled = t->next;
            delete t;
            t = sled;
        }
        start = nullptr;
        end = nullptr;
    }
    Polinom& operator=(Polinom obj){
        this->clear();
        Link* t = obj.start;
        while(t!= nullptr){
            this->add(t->m);
            t = t->next;
        }
        return *this;
    }
    Polinom operator*(double x){
        Polinom ans(*this);
        Link* t = ans.start;
        while(t!= nullptr){
            t->m.c*=x;
            t = t->next;
        }
        return ans;
    }
    Polinom operator+(Polinom& obj){
        Polinom ans;
        Link* l = start;
        Link* r = obj.start;
        Monom x;
        while(true){
            if(r == nullptr && l== nullptr) {
                break;
            }
            if(l == nullptr){
                x = r->m;
                r = r->next;
            }
            else if(r == nullptr){
                x = l->m;
                l = l->next;
            }
            else if(l->m.pow == r->m.pow){
                x.pow = l->m.pow;
                x.c = l->m.c + r->m.c;
                l = l->next;
                r = r->next;
            }
            else if(l->m.pow < r->m.pow){
                x = l->m;
                l = l->next;
            }
            else if(l->m.pow > r->m.pow){
                x = r->m;
                r = r->next;
            }
            if(x.c!=0) {
                ans.add(x);
            }
        }
        return ans;
    }
    Polinom operator-(Polinom& obj){
        Polinom tmp = obj * (-1);

        return (*this) + (tmp);
    }
    Polinom operator*(Polinom& obj){
        //Сложность O( n*m*log(n*m) )
        //Если степени монома окажутся вне пределах l, r , то этот моном мы не добавляем
        Polinom ans;
        Link* l = this->start;
        map<int,int>koef;
        while(l!= nullptr){
            Link* r = obj.start;
            while(r!= nullptr){
                Monom x = l->m * r->m;
                if(x.pow != -1)
                    koef[x.pow]+=x.c;
                r = r->next;
            }
            l = l->next;
        }
        for(auto x:koef){
            if(x.second != 0) {
                //вставка за 1, так как у нас будут степени в мапе по возрастанию
                ans.add(Monom(get_X(x.first), get_Y(x.first),get_Z(x.first),x.second));
            }
        }
        return ans;
    }
    bool operator == (Polinom& obj){
        Link* l = start;
        Link* r = obj.start;
        while(l!= nullptr){
            if(l->m != r->m)
                return false;
            l = l->next;
            r = r->next;
        }
        if(r != nullptr)
            return false;
        return true;
    }
    bool operator != (Polinom& obj){
        if(*this == obj)
            return false;
        return true;
    }
    friend ostream& operator<<(ostream& out , const Polinom& obj){
        Link* t =obj.start;
        if(t == nullptr){
            out<<"Polinom Is Empty\n";
            return out;
        }
        while(t != nullptr){
            out<<t->m<<' ';
            t = t->next;
        }
        return out;
    }
    friend istream& operator>>(istream& in , Polinom& obj){
        obj.clear();
        string s;
        getline(in,s);
        string t = "";
        int koef = 1;
        for(int i=0 ;i<s.size();i++){
            if(s[i] == ' '){
                if(t!=""){
                    Monom x(t);
                    x.c*=koef;
                    obj.add(x);
                    koef = 1;
                }
                t = "";
            }
            else if(s[i] == '+'){
                koef = 1;
            }
            else if(s[i] =='-' && i>0 && s[i-1] =='^')
                t+=s[i];
            else if(s[i]=='-'){
                koef = -1;
            }
            else
                t+=s[i];
        }
        if(t!="") {
            Monom x(t);
            x.c*=koef;
            obj.add(x);
        }
        return in;
    }
};



