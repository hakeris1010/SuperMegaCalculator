#ifndef SKAICIUOTUV_H_INCLUDED
#define SKAICIUOTUV_H_INCLUDED

class Skaiciuotuvas
{
private:
    int mod=1;

    void papr();
    void sud();
    void input();
    double skaic_veiksm();
    double spec_veiksm();

public:
    Skaiciuotuvas(int mode);

    void run();
    void set_mode(int mode);
};

void paskaiciuok();

#endif // SKAICIUOTUV_H_INCLUDED
