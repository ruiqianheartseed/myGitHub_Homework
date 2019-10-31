#include "stdafx.h"
#include "transform.h"
#include "math.h"
#define PI 3.1415926
float *fftBuf=0;
void fft1D(float *arrayBuf, int n)
{
	int i, j, k, r;
	float *buf1=new float[n*2];
	for(i=0;i<n*2;i++)
		buf1[i]=arrayBuf[i];

	float *buf2=new float[n*2];
	int t1,t2;
	for(r=1;pow(2.0,r)<n;r++){
		t1=pow(2.0,r);
		t2=pow(2.0,r-1);
		for(k=0;k<t1;k++){
			for(i=0;i<n/t1;i++){				
				buf2[k*n/t1*2+i*2+0]=buf1[k/2*n/t2*2+(i*2+k%2)*2+0];
				buf2[k*n/t1*2+i*2+1]=buf1[k/2*n/t2*2+(i*2+k%2)*2+1];
			}
		}
		for(i=0;i<n*2;i++)
			buf1[i]=buf2[i];
	}
	float c,s,sh,xu;
	for(r=1;pow(2.0,r)<=n;r++){
		t1=pow(2.0,r);
		for(k=0;k<n/t1;k++){
			for(i=t1/2;i<t1;i++){
			//	c=cos(-2*PI*(i-t1/2)/t1);
			//	s=sin(-2*PI*(i-t1/2)/t1);
				c=cos(2*PI*(-i+t1/2+n/2)/t1);				
				s=sin(2*PI*(-i+t1/2+n/2)/t1);
				buf1[k*t1*2+i*2+0]= buf2[k*t1*2+i*2+0]*c - buf2[k*t1*2+i*2+1]*s;				
				buf1[k*t1*2+i*2+1]=buf2[k*t1*2+i*2+1]*c+buf2[k*t1*2+i*2+0]*s;
			}
		}
		
		for(k=0; k<n/t1; k++){
			for(i=0;i<t1/2;i++){
				buf2[k*t1*2+i*2+0]= buf1[k*t1*2+i*2+0]+buf1[k*t1*2+(i+t1/2)*2+0];
				buf2[k*t1*2+i*2+1]= buf1[k*t1*2+i*2+1]+buf1[k*t1*2+(i+t1/2)*2+1];
			}
			for(i=t1/2;i<t1;i++){
				buf2[k*t1*2+i*2+0]= buf1[k*t1*2+(i-t1/2)*2+0]-buf1[k*t1*2+i*2+0];
				buf2[k*t1*2+i*2+1]= buf1[k*t1*2+(i-t1/2)*2+1]-buf1[k*t1*2+i*2+1];
			}
		}

		for(i=0;i<n*2;i++)
			buf1[i]=buf2[i];
	}
	
	for(i=0;i<n*2;i++)
			arrayBuf[i]=buf2[i];

	delete[]buf2;
	delete[]buf1;
}

void fft2D(unsigned char* imgBuf, int width, int height)
{
	int i, j, u, v;
	float *buf=new float[width*height*2];
	for(i=0;i<width*height;i++){
		buf[i*2+0]=imgBuf[i];
		buf[i*2+1]=0;
	}

	float *array=new float[height*2];
	for(u=0;u<width;u++){
		for(v=0;v<height;v++){
			array[v*2+0]=buf[v*width*2+u*2+0];
			array[v*2+1]=buf[v*width*2+u*2+1];
		}
		fft1D(array, width);
		for(v=0;v<height;v++){
			buf[v*width*2+u*2+0]=array[v*2+0];
			buf[v*width*2+u*2+1]=array[v*2+1];
		}
	}
	delete []array;

	for(v=0;v<height;v++){
		fft1D(buf+v*width*2, width);
	}
	//频域数据保存至fftBuf
    fftBuf=buf;

	//修改频域数据
	int off;

	//显示需要
	float *buf1=new float[width*height];
	for(i=0;i<width*height;i++)
		buf1[i]= sqrt(buf[i*2+0]*buf[i*2+0]+buf[i*2+1]*buf[i*2+1]);

	int mo=2000;//模值
	for(i=0;i<width*height;i++){
		if(buf1[i]/mo>255)
		   imgBuf[i]= 255;
		else imgBuf[i]=buf1[i]/mo;
	}
	

	

	delete[]buf1;

}
void fft2D_Cutnoise(unsigned char* imgBuf, int width, int height)
{
	int i, j, u, v;
	float *buf=new float[width*height*2];
	for(i=0;i<width*height;i++){
		buf[i*2+0]=imgBuf[i];
		buf[i*2+1]=0;
	}
	float *array=new float[height*2];
	for(u=0;u<width;u++){
		for(v=0;v<height;v++){
			array[v*2+0]=buf[v*width*2+u*2+0];
			array[v*2+1]=buf[v*width*2+u*2+1];
		}
		fft1D(array, width);
		for(v=0;v<height;v++){
			buf[v*width*2+u*2+0]=array[v*2+0];
			buf[v*width*2+u*2+1]=array[v*2+1];
		}
	}
	delete []array;
	for(v=0;v<height;v++){
		fft1D(buf+v*width*2, width);
	}
	//频域数据保存至fftBuf
    fftBuf=buf;

	//修改频域数据
	int off;

	//带阻去噪
	for(i=height/2-5; i<=height/2+5;i++){
		for(j=width/2-10; j<= width/2-5;j++){
			*(fftBuf+i*width*2+j*2+0)=*(fftBuf+i*width*2+j*2+0)/10;
			*(fftBuf+i*width*2+j*2+1)=*(fftBuf+i*width*2+j*2+1)/10;
		}
		for(j=width/2+5; j<= width/2+10;j++){
			*(fftBuf+i*width*2+j*2+0)=*(fftBuf+i*width*2+j*2+0)/10;
			*(fftBuf+i*width*2+j*2+1)=*(fftBuf+i*width*2+j*2+1)/10;
		}
	}


	//显示需要
	float *buf1=new float[width*height];
	for(i=0;i<width*height;i++)
		buf1[i]= sqrt(buf[i*2+0]*buf[i*2+0]+buf[i*2+1]*buf[i*2+1]);

	int mo=2000;//模值
	for(i=0;i<width*height;i++){
		if(buf1[i]/mo>255)
		   imgBuf[i]= 255;
		else imgBuf[i]=buf1[i]/mo;
	}
	delete[]buf1;

}

/*
void fft2D_Noise(unsigned char* imgBuf, int width, int height)//平面正弦噪声
{
	int i, j, u, v;
	float *buf=new float[width*height*2];
	for(i=0;i<width*height;i++){
		buf[i*2+0]=imgBuf[i];
		buf[i*2+1]=0;
	}
	float *array=new float[height*2];
	for(u=0;u<width;u++){
		for(v=0;v<height;v++){
			array[v*2+0]=buf[v*width*2+u*2+0];
			array[v*2+1]=buf[v*width*2+u*2+1];
		}
		fft1D(array, width);
		for(v=0;v<height;v++){
			buf[v*width*2+u*2+0]=array[v*2+0];
			buf[v*width*2+u*2+1]=array[v*2+1];
		}
	}
	delete []array;

	for(v=0;v<height;v++){
		fft1D(buf+v*width*2, width);
	}
	//频域数据保存至fftBuf
	fftBuf=buf;

	//修改频域数据
	int off;
	for(j=width/2-5; j<= width/2+5;j++){
		for(i=height/2-10; i<=height/2-5;i++){
			*(fftBuf+i*width+j+0)=*(fftBuf+i*width+j+0)*10;
			*(fftBuf+i*width+j+1)=*(fftBuf+i*width+j+1)*10;
		}
		for(i=height/2+5; i<=height/2+10;i++){
			*(fftBuf+i*width+j+0)=*(fftBuf+i*width+j+0)*10;
			*(fftBuf+i*width+j+1)=*(fftBuf+i*width+j+1)*10;
		}
	}

	//显示需要
	float *buf1=new float[width*height];
	for(i=0;i<width*height;i++)
		buf1[i]= sqrt(buf[i*2+0]*buf[i*2+0]+buf[i*2+1]*buf[i*2+1]);

	int mo=2000;//模值
	for(i=0;i<width*height;i++){
		if(buf1[i]/mo>255)
			imgBuf[i]= 255;
		else imgBuf[i]=buf1[i]/mo;
	}
	delete[]buf1;

}
*/

void ifft1D(float *arrayBuf, int n)
{
	int i, j, k, r;
	float *buf1=new float[n*2];
	for(i=0;i<n*2;i++)
		buf1[i]=arrayBuf[i];

	float *buf2=new float[n*2];
	int t1,t2;
	for(r=1;pow(2.0,r)<n;r++){
		t1=pow(2.0,r);
		t2=pow(2.0,r-1);
		for(k=0;k<t1;k++){
			for(i=0;i<n/t1;i++){				
				buf2[k*n/t1*2+i*2+0]=buf1[k/2*n/t2*2+(i*2+k%2)*2+0];
				buf2[k*n/t1*2+i*2+1]=buf1[k/2*n/t2*2+(i*2+k%2)*2+1];
			}
		}
		for(i=0;i<n*2;i++)
			buf1[i]=buf2[i];
	}
	float c,s,sh,xu;
	for(r=1;pow(2.0,r)<=n;r++){
		t1=pow(2.0,r);
		for(k=0;k<n/t1;k++){
			for(i=t1/2;i<t1;i++){
			//	c=cos(-2*PI*(i-t1/2)/t1);
			//	s=sin(-2*PI*(i-t1/2)/t1);
				c=cos(2*PI*(-i+t1/2)/t1);				
				s=sin(2*PI*(-i+t1/2)/t1);
				buf1[k*t1*2+i*2+0]= buf2[k*t1*2+i*2+0]*c - buf2[k*t1*2+i*2+1]*s;				
				buf1[k*t1*2+i*2+1]=buf2[k*t1*2+i*2+1]*c+buf2[k*t1*2+i*2+0]*s;
			}
		}
		
		for(k=0; k<n/t1; k++){
			for(i=0;i<t1/2;i++){
				buf2[k*t1*2+i*2+0]= buf1[k*t1*2+i*2+0]+buf1[k*t1*2+(i+t1/2)*2+0];
				buf2[k*t1*2+i*2+1]= buf1[k*t1*2+i*2+1]+buf1[k*t1*2+(i+t1/2)*2+1];
			}
			for(i=t1/2;i<t1;i++){
				buf2[k*t1*2+i*2+0]= buf1[k*t1*2+(i-t1/2)*2+0]-buf1[k*t1*2+i*2+0];
				buf2[k*t1*2+i*2+1]= buf1[k*t1*2+(i-t1/2)*2+1]-buf1[k*t1*2+i*2+1];
			}
		}

		for(i=0;i<n*2;i++)
			buf1[i]=buf2[i];
	}
	
	for(i=0;i<n*2;i++)
			arrayBuf[i]=buf2[i];

	delete[]buf2;
	delete[]buf1;
}

void ifft2D(unsigned char* imgBuf, int width, int height)
{
	if(!fftBuf)
		return;

//	cutNoise(fftBuf, width, height);
	int i, j, u, v;
	float *buf=fftBuf;
	for(i=0;i<width*height;i++)
		buf[i*2+1] = -buf[i*2+1];

	float *array=new float[height*2];
	for(u=0;u<width;u++){
		for(v=0;v<height;v++){
			array[v*2+0]=buf[v*width*2+u*2+0];
			array[v*2+1]=buf[v*width*2+u*2+1];
		}
		ifft1D(array, width);
		for(v=0;v<height;v++){
			buf[v*width*2+u*2+0]=array[v*2+0];
			buf[v*width*2+u*2+1]=array[v*2+1];
		}
	}
	delete []array;

	for(v=0;v<height;v++){
		ifft1D(buf+v*width*2, width);
	}

	float *buf1=new float[width*height];
	for(i=0;i<width*height;i++){
		int j=sqrt(buf[i*2+0]*buf[i*2+0]+buf[i*2+1]*buf[i*2+1]);
		buf1[i]= j/(width*height);
	}
	int max=buf1[0], min=buf1[0];
	for(i=0;i<width*height;i++){
		if(buf1[i]>max) max=buf1[i];
		if(buf1[i]<min) min=buf1[i];
	}
	if(max<255){
		for(i=0;i<width*height;i++)
		   imgBuf[i]=buf1[i];
	}
	else{
		for(i=0;i<width*height;i++){
			if(buf1[i]>255)
				imgBuf[i]=255;
	    	else imgBuf[i]=buf1[i];//*255/max;
		}
	}

	delete[]buf;
	delete[]buf1;

}


