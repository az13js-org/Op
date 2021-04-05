#include "Op.h"
#include "GNode/Node.h"
#include "GNode/Tree.h"
#include <queue>
#include <iostream>

using namespace std;
using namespace GNode;

int main() {
    int operationOpTotal = 100;
    int numberOpTotal = operationOpTotal + 2;
    float min = 0.5;
    float max = 6.0;
    int offset = 1;
    auto gepSequence = new Node<Op*>*[operationOpTotal + numberOpTotal];
    queue<Node<Op*>*> hungryQueue;
    Node<Op*>* workingNode;
    Node<Op*>* childNode;
    Op* childNodeOp;
    int emptyChildNumber;
    Op* rootOp = Op::getRandomOptionOp();
    while (Op::END == rootOp->getTypeValue()) {
        delete rootOp;
        rootOp = Op::getRandomOptionOp();
    }
    auto tree = new Tree<Op*>(rootOp, [](Op* o){delete o;});
    // 随机地构建基因序列，开头的都是运算操作符号，后面的都是固定的数字符号
    gepSequence[0] = tree->getRoot();
    for (int i = 1; i < operationOpTotal; i++) {
        gepSequence[i] = tree->create(Op::getRandomOptionOp());
    }
    for (int i = operationOpTotal; i < operationOpTotal + numberOpTotal; i++) {
        gepSequence[i] = tree->create(Op::getRandomNumberOp(min, max));
    }
    // 构建图
    hungryQueue.push(gepSequence[0]);
    while (!hungryQueue.empty()) {
        workingNode = hungryQueue.front();
        hungryQueue.pop();
        emptyChildNumber = workingNode->getValue()->getOperandTotal();
        while (emptyChildNumber > 0) {
            childNodeOp = gepSequence[offset]->getValue();
            if (Op::OP_OPERATION == childNodeOp->getOpType()) {
                if (Op::END != childNodeOp->getTypeValue()) {
                    childNode = gepSequence[offset];
                    hungryQueue.push(childNode);
                } else {
                    offset = operationOpTotal;
                    childNodeOp = gepSequence[operationOpTotal]->getValue();
                    childNode = gepSequence[operationOpTotal];
                }
            } else {
                childNode = gepSequence[offset];
            }
            childNodeOp->setOpAttribute(
                workingNode->getValue()->getOperandTotal() - emptyChildNumber == 0 ?
                Op::OP_ATTR_LEFT : Op::OP_ATTR_RIGHT
            );
            workingNode->add(childNode);
            offset++;
            emptyChildNumber--;
            if (offset == operationOpTotal + numberOpTotal) {
                cout << "Error, out of size" << endl;
                return -1;
            }
        }
    }
    // 打印图中存储的内容
    rootOp->print(tree->getRoot());
    cout << "=" << rootOp->calculate(tree->getRoot()) << endl;
    cout << endl;
    delete[] gepSequence;
    delete tree;
    return 0;
}
