#ifndef OP_H
#define OP_H

#include "GNode/Node.h"
#include <random>
#include <iostream>

class Op {

public:

    static const int ADD; // 加
    static const int SUB; // 减
    static const int PRO; // 乘
    static const int DES; // 除
    static const int END; // 停止操作

    static const int OP_OPERATION; // 运算符
    static const int OP_NUMBER; // 数字

    static const int OP_ATTR_LEFT; // 是左侧的数字
    static const int OP_ATTR_RIGHT; // 是右侧的数字

private:

    static std::default_random_engine e;

    int opType; // OP_OPERATION or OP_NUMBER

    float opNumber; // value of op if OP_NUMBER

    int opTypeNumber; // op type value if OP_OPERATION

    int numberLeftOrRight; // 是左侧的还是右侧的

public:

    static Op* getRandomNumberOp(float min, float max) {
        using namespace std;
        uniform_real_distribution<float> realDistribution(min, max);
        return new Op(Op::OP_NUMBER, realDistribution(e));
    }

    static Op* getRandomOptionOp() {
        using namespace std;
        uniform_int_distribution<int> opTypeDistribution(ADD, END);
        return new Op(Op::OP_OPERATION, opTypeDistribution(e));
    }

    static Op* getRandomOp(float min, float max) {
        using namespace std;
        bernoulli_distribution boolDistribution(0.5);
        if (boolDistribution(e)) { // is OP_NUMBER
            return getRandomNumberOp(min, max);
        }
        return getRandomOptionOp();
    }

    Op(int opOpType, float number) {
        opType = opOpType;
        opNumber = number;
    }

    Op(int opOpType, int number) {
        opType = opOpType;
        opTypeNumber = number;
    }

    int getOpType() {
        return opType;
    }

    float getValue() {
        return opNumber;
    }

    int getTypeValue() {
        return opTypeNumber;
    }

    void setOpAttribute(int opNumberAttribute) {
        numberLeftOrRight = opNumberAttribute;
    }

    int getOpAttribute() {
        return numberLeftOrRight;
    }

    void print(GNode::Node<Op*>* node) {
        using namespace std;
        if (OP_NUMBER == opType) {
            cout << opNumber;
            return;
        }
        cout << "(";
        for (auto e : node->getNodes()) {
            if (OP_ATTR_LEFT == e->getValue()->getOpAttribute()) {
                e->getValue()->print(e);
                break;
            }
        }
        if (ADD == opTypeNumber) {
            cout << "+";
        }
        if (SUB == opTypeNumber) {
            cout << "-";
        }
        if (PRO == opTypeNumber) {
            cout << "*";
        }
        if (DES == opTypeNumber) {
            cout << "/";
        }
        for (auto e : node->getNodes()) {
            if (OP_ATTR_RIGHT == e->getValue()->getOpAttribute()) {
                e->getValue()->print(e);
                break;
            }
        }
        cout << ")";
    }

    float calculate(GNode::Node<Op*>* node) {
        using namespace std;
        if (OP_NUMBER == opType) {
            return opNumber;
        }
        float left = 0;
        float right = 0;
        for (auto e : node->getNodes()) {
            if (OP_ATTR_LEFT == e->getValue()->getOpAttribute()) {
                left = e->getValue()->calculate(e);
            } else {
                right = e->getValue()->calculate(e);
            }
        }
        if (ADD == opTypeNumber) {
            return left + right;
        }
        if (SUB == opTypeNumber) {
            return left - right;
        }
        if (PRO == opTypeNumber) {
            return left * right;
        }
        if (right < 0.000001) {
            return 0;
        }
        return left / right;
    }

};

const int Op::ADD = 1;
const int Op::SUB = 2;
const int Op::PRO = 3;
const int Op::DES = 4;
const int Op::END = 5;

const int Op::OP_OPERATION = -1;
const int Op::OP_NUMBER = -2;

const int Op::OP_ATTR_LEFT = -3;
const int Op::OP_ATTR_RIGHT = -4;

std::default_random_engine Op::e;

#endif
