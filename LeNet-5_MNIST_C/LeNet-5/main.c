#include "lenet.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define FILE_TRAIN_IMAGE		"train-images-idx3-ubyte"
#define FILE_TRAIN_LABEL		"train-labels-idx1-ubyte"
#define FILE_TEST_IMAGE		"t10k-images-idx3-ubyte"
#define FILE_TEST_LABEL		"t10k-labels-idx1-ubyte"
#define LENET_FILE 		"model.dat"
#define COUNT_TRAIN		60000 //训练集实际大小
#define COUNT_TEST		10000 //测试集实际大小
#define TRAIN_NUMBER    6*COUNT_TRAIN//期望的训练集迭代次数


int read_data(unsigned char(*data)[28][28], unsigned char label[], const int count, const char data_file[], const char label_file[])
{
	/*
	//配置1，方便训练时连续读取
	int turn = count / COUNT_TRAIN;//读取次数
	int temp = count % COUNT_TRAIN;//剩余次数
	const int ALL = COUNT_TRAIN;

	if (turn >= 1)
	{
		for (int i = 0; i < turn; i++)
		{
			FILE *fp_image = fopen(data_file, "rb");//读图片地址
			FILE *fp_label = fopen(label_file, "rb");//读标签地址
			//if (!fp_image||!fp_label) return 1;//如果文件不存在返回1
			fseek(fp_image, 16, SEEK_SET);//跳过.gz图片文件的头部分
			fseek(fp_label, 8, SEEK_SET);//跳过.gz标签文件的头部分
			fread(data + (i*ALL), sizeof(*data)*ALL, 1, fp_image);//读图片
			fread(label + (i*ALL), ALL, 1, fp_label);//读标签
			fclose(fp_image);//关闭图片
			fclose(fp_label);//关闭标签
		}
	}
	FILE *fp_image = fopen(data_file, "rb");//读图片地址
	FILE *fp_label = fopen(label_file, "rb");//读标签地址
	//if (!fp_image || !fp_label) return 1;//如果文件不存在返回1
	fseek(fp_image, 16, SEEK_SET);//跳过.gz图片文件的头部分
	fseek(fp_label, 8, SEEK_SET);//跳过.gz标签文件的头部分
	fread(data + (turn*ALL), sizeof(*data)*temp, 1, fp_image);//读图片
	fread(label + (turn*ALL), temp, 1, fp_label);//读标签
	fclose(fp_image);//关闭图片
	fclose(fp_label);//关闭标签
	return 0;
	*/
	//配置2，节约空间复杂度
	FILE *fp_image = fopen(data_file, "rb");//读图片地址
	FILE *fp_label = fopen(label_file, "rb");//读标签地址
	//if (!fp_image || !fp_label) return 1;//如果文件不存在返回1
	fseek(fp_image, 16, SEEK_SET);//跳过.gz图片文件的头部分
	fseek(fp_label, 8, SEEK_SET);//跳过.gz标签文件的头部分
	fread(data, sizeof(*data)*count, 1, fp_image);//读图片
	fread(label, count, 1, fp_label);//读标签
	fclose(fp_image);//关闭图片
	fclose(fp_label);//关闭标签
	return 0;
	/*
	
	*/
}

void training(LeNet5 *lenet, image *train_data, uint8 *train_label, int batch_size, int total_size, int *ac) 
{
	/*
	
	*/
	//配置2
	int turn = total_size / (COUNT_TRAIN / batch_size * batch_size);//读取次数
	int temp = total_size % (COUNT_TRAIN / batch_size * batch_size);//剩余次数
	ac[0] = 0;
	if (turn>=1)
	{
		for (int t = 0; t < turn; t++)
		{
			for (int i = 0, percent = 0; i <= COUNT_TRAIN - batch_size; i += batch_size) //i标记batch起点位置
			{
				TrainBatch(lenet, train_data + i, train_label + i, batch_size);
				if ((i + t * (COUNT_TRAIN / batch_size * batch_size)) * 100 / total_size > percent)
				{
					printf("batchsize:%d\ttrain:%2d%%", batch_size, percent = (i + t * (COUNT_TRAIN / batch_size * batch_size)) * 100 / total_size);
					int right = testing(lenet, train_data + i, train_label + i, batch_size);
					printf("  train accuracy: %d%%\n", ac[percent] =(((double)right / (double)batch_size) * 100));
				}	
			}
		}
	}
	if (temp >= batch_size)
	{
		for (int i = 0, percent = (total_size-temp)*100/total_size; i <= temp - batch_size; i += batch_size) //i标记batch起点位置
			{
				TrainBatch(lenet, train_data + i, train_label + i, batch_size); 
				if ((i + turn * (COUNT_TRAIN / batch_size * batch_size)) * 100 / total_size > percent)
				{
					printf("batchsize:%d\ttrain:%2d%%", batch_size, percent = (i + turn*(COUNT_TRAIN/batch_size*batch_size))* 100 / total_size);
					int right = testing(lenet, train_data + i, train_label + i, batch_size);
					printf("  train accuracy: %d%%\n", ac[percent] =(((double)right / (double)batch_size) * 100));
				}
			}
	}
	/*
		//配置1
		for (int i = 0, percent = 0; i <= total_size - batch_size; i += batch_size) //i标记batch起点位置
	{
		TrainBatch(lenet, train_data + i, train_label + i, batch_size);
		if (i * 100 / total_size > percent)
			printf("batchsize:%d\ttrain:%2d%%\n", batch_size, percent = i * 100 / total_size);
	}
	*/
}

int testing(LeNet5 *lenet, image *test_data, uint8 *test_label,int total_size)
{
	int right = 0, percent = 0;
	for (int i = 0; i < total_size; ++i)
	{
		uint8 l = test_label[i];
		int p = Predict(lenet, test_data[i], 10);
		right += l == p;
		//if (i * 100 / total_size > percent) printf("test:%2d%%\n", percent = i * 100 / total_size);
	}
	return right;
}

int save(LeNet5 *lenet, char filename[])
{
	FILE *fp = fopen(filename, "wb");
	if (!fp) return 1;
	fwrite(lenet, sizeof(LeNet5), 1, fp);
	fclose(fp);
	return 0;
}

int load(LeNet5 *lenet, char filename[])
{
	FILE *fp = fopen(filename, "rb");
	if (!fp) return 1;//不存在数据参数则返回1，重新初始化整个网络
	fread(lenet, sizeof(LeNet5), 1, fp);
	fclose(fp);
	return 0;
}

void foo()
{
	//image *train_data = (image *)calloc(TRAIN_NUMBER, sizeof(image));
	//uint8 *train_label = (uint8 *)calloc(TRAIN_NUMBER, sizeof(uint8));
	image *train_data = (image *)calloc(COUNT_TRAIN, sizeof(image));
	uint8 *train_label = (uint8 *)calloc(COUNT_TRAIN, sizeof(uint8));
	image *test_data = (image *)calloc(COUNT_TEST, sizeof(image));
	uint8 *test_label = (uint8 *)calloc(COUNT_TEST, sizeof(uint8));
	//if (read_data(train_data, train_label, TRAIN_NUMBER, FILE_TRAIN_IMAGE, FILE_TRAIN_LABEL))//配置1
	if (read_data(train_data, train_label, COUNT_TRAIN, FILE_TRAIN_IMAGE, FILE_TRAIN_LABEL))//配置2
	{
		printf("ERROR!!!\nDataset File Not Find!Please Copy Dataset to the Floder Included the exe\n");
		free(train_data);
		free(train_label);
		system("pause");
	}
	if (read_data(test_data, test_label, COUNT_TEST, FILE_TEST_IMAGE, FILE_TEST_LABEL))
	{
		printf("ERROR!!!\nDataset File Not Find!Please Copy Dataset to the Floder Included the exe\n");
		free(test_data);
		free(test_label);
		system("pause");
	}
	LeNet5 *lenet = (LeNet5 *)malloc(sizeof(LeNet5));
	//if (load(lenet, LENET_FILE)) Initial(lenet);//可以迭代上一次结果
	Initial(lenet);//为了保证每次结果相同暂且使用统一随机初始化
	clock_t start = clock();
	int batches[] = { 100 };//可调参数minibatch
	int ac[100] = { 0, };//储存train accuracy
	for (int i = 0; i < sizeof(batches) / sizeof(*batches); ++i)
	{
		training(lenet, train_data, train_label, batches[i], TRAIN_NUMBER, ac);
	}
	printf("testing---------");
	int right = testing(lenet, test_data, test_label, COUNT_TEST);
	printf("test accuracy: %d/%d\n", right, COUNT_TEST);
	printf("Time:%u\n", (unsigned)(clock() - start));
	//save(lenet, LENET_FILE);//有必要反复迭代的话存起来参数
	free(lenet);
	free(train_data);
	free(train_label);
	free(test_data);
	free(test_label);
	system("pause");
}

int main()
{
	foo();
	return 0;
}