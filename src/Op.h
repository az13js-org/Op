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

    float opNumberMin = 0.0;

    float opNumberMax = 1.0;

public:

    static Op* getRandomNumberOp(float min, float max) {
        using namespace std;
        uniform_real_distribution<float> realDistribution(min, max);
        return new Op(Op::OP_NUMBER, realDistribution(e), min, max);
    }

    static Op* getRandomOptionOp() {
        using namespace std;
        uniform_int_distribution<int> opTypeDistribution(ADD, END);
        return new Op(Op::OP_OPERATION, opTypeDistribution(e));
    }

    static Op* getRandomOp(float min = 0.0, float max = 1.0) {
        using namespace std;
        bernoulli_distribution boolDistribution(0.5);
        if (boolDistribution(e)) { // is OP_NUMBER
            return getRandomNumberOp(min, max);
        }
        return getRandomOptionOp();
    }

    static Op* createLike(Op* source) {
        if (OP_OPERATION == source->getOpType()) {
            return new Op(source->getOpType(), source->getTypeValue());
        }
        return new Op(source->getOpType(), source->getValue(), source->getMin(), source->getMax());
    }

    Op(int opOpType, float number, float min = 0.0, float max = 1.0) {
        opType = opOpType;
        opNumber = number;
        opNumberMin = min;
        opNumberMax = max;
    }

    Op(int opOpType, int number) {
        opType = opOpType;
        opTypeNumber = number;
    }

    int getOpType() {
        return opType; // OP_OPERATION, OP_NUMBER
    }

    float getValue() {
        return opNumber;
    }

    int getTypeValue() {
        return opTypeNumber; // ADD, SUB, PRO, DES, END
    }

    void setOpAttribute(int opNumberAttribute) {
        numberLeftOrRight = opNumberAttribute;
    }

    int getOpAttribute() {
        return numberLeftOrRight;
    }

    float getMin() {
        return opNumberMin;
    }

    float getMax() {
        return opNumberMax;
    }

    /**
     * 返回当前的 Op 需要的子 Op 数目
     *
     * @return int
     */
    int getOperandTotal() {
        if (OP_NUMBER == opType) {
            return 0;
        }
        return 2;
    }

    void print(GNode::Node<Op*>* node) {
        using namespace std;
        using namespace GNode;
        if (OP_NUMBER == opType) {
            if (opNumber < 0) {
                cout << "(";
            }
            cout << opNumber;
            if (opNumber < 0) {
                cout << ")";
            }
            return;
        }
        Op* left = nullptr;
        Op* right = nullptr;
        Node<Op*>* nodeLeft = nullptr;
        Node<Op*>* nodeRight = nullptr;
        for (auto e : node->getNodes()) {
            if (OP_ATTR_LEFT == e->getValue()->getOpAttribute()) {
                left = e->getValue();
                nodeLeft = e;
            }
            if (OP_ATTR_RIGHT == e->getValue()->getOpAttribute()) {
                right = e->getValue();
                nodeRight = e;
            }
        }
        if (nullptr == left || nullptr == right) {
            cout << "?";
            return;
        }
        cout << "(";
        left->print(nodeLeft);
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
        right->print(nodeRight);
        cout << ")";
    }

    /**
     * 执行表达式计算
     *
     * @param Node<Op*>* node 节点
     * @return float
     */
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
        if (right < 1E-6) {
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
