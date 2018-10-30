#ifndef PAPAI_NOEL_HPP
#define PAPAI_NOEL_HPP


class PapaiNoel {
private:
    bool dormindo;
public:
    PapaiNoel();
    ~PapaiNoel();
    bool estaDormindo();
    void acordar();
    void dormir();
};


#endif
