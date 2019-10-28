/*
根据Yann Lecun的论文《Gradient-based Learning Applied To Document Recognition》编写:
批量训练
void TrainBatch(LeNet5 *lenet, image *inputs, uint8 *labels, int batchSize);
预测
uint8 Predict(LeNet5 *lenet, image input, uint8 count);
初始化
void Initial(LeNet5 *lenet);
*/
#pragma once
#define LENGTH_KERNEL	5

#define LENGTH_FEATURE0	32//输入时加了两圈padding
#define LENGTH_FEATURE1	(LENGTH_FEATURE0 - LENGTH_KERNEL + 1)//28
#define LENGTH_FEATURE2	(LENGTH_FEATURE1 >> 1)//14
#define LENGTH_FEATURE3	(LENGTH_FEATURE2 - LENGTH_KERNEL + 1)//10
#define	LENGTH_FEATURE4	(LENGTH_FEATURE3 >> 1)//5
#define LENGTH_FEATURE5	(LENGTH_FEATURE4 - LENGTH_KERNEL + 1)//1

#define INPUT			1
#define LAYER1			6      //6个5*5卷积核
#define LAYER2			6      //最大池化
#define LAYER3			16     //16个5*5卷积核
#define LAYER4			16     //最大池化
#define LAYER5			120    //全连接层节点数120
#define LAYER6          84     //全连接层节点数84
#define OUTPUT          10     //输出分类

#define REAL_ALPHA      0.001//单位学习率
#define ALPHA           REAL_ALPHA*100 //minibatch梯度下降法的总batch学习率
#define PADDING         2 //为计算简便，在不影响后续识别准确度的情况下，只在第0输入层padding了

typedef unsigned char uint8;
typedef uint8 image[28][28];

typedef struct LeNet5
{
	double weight0_1[INPUT][LAYER1][LENGTH_KERNEL][LENGTH_KERNEL];
	double weight2_3[LAYER2][LAYER3][LENGTH_KERNEL][LENGTH_KERNEL];
	double weight4_5[LAYER4][LAYER5][LENGTH_KERNEL][LENGTH_KERNEL];
	double weight5_6[LAYER5][LAYER6];
	double weight6_7[LAYER6][OUTPUT];

	double bias0_1[LAYER1];
	double bias2_3[LAYER3];
	double bias4_5[LAYER5];
	double bias5_6[LAYER6];
	double bias6_7[OUTPUT];

}LeNet5;

typedef struct Feature
{
	double input[INPUT][LENGTH_FEATURE0][LENGTH_FEATURE0];
	double layer1[LAYER1][LENGTH_FEATURE1][LENGTH_FEATURE1];
	double layer2[LAYER2][LENGTH_FEATURE2][LENGTH_FEATURE2];
	double layer3[LAYER3][LENGTH_FEATURE3][LENGTH_FEATURE3];
	double layer4[LAYER4][LENGTH_FEATURE4][LENGTH_FEATURE4];
	double layer5[LAYER5][LENGTH_FEATURE5][LENGTH_FEATURE5];
	double layer6[LAYER6];
	double output[OUTPUT];
}Feature;

void TrainBatch(LeNet5 *lenet, image *inputs, uint8 *labels, int batchSize);

uint8 Predict(LeNet5 *lenet, image input, uint8 count);

void Initial(LeNet5 *lenet);
