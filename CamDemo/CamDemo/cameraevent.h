#ifndef CAMERAEVENT_H
#define CAMERAEVENT_H
#include <QDebug>
#include"GalaxyIncludes.h"

//用户继承掉线事件处理类

class CSampleDeviceOfflineEventHandler : public IDeviceOfflineEventHandler

{

public:

   void DoOnDeviceOfflineEvent(void* pUserParam)

   {

       qDebug()<<"收到设备掉线事件!"<<endl;

   }

};



//用户继承属性更新事件处理类

class CSampleFeatureEventHandler : public IFeatureEventHandler

{

public:

   void DoOnFeatureEvent(const GxIAPICPP::gxstring&strFeatureName, void* pUserParam)

   {

       qDebug()<<"收到曝光结束事件!"<<endl;

   }

};



//用户继承采集事件处理类

class CSampleCaptureEventHandler : public ICaptureEventHandler

{

public:

   void DoOnImageCaptured(CImageDataPointer&objImageDataPointer, void* pUserParam)

   {

       qDebug()<<"收到一帧图像!"<<endl;

       qDebug()<<"ImageInfo: "<<objImageDataPointer->GetStatus() <<endl;

       qDebug()<<"ImageInfo: "<<objImageDataPointer->GetWidth() <<endl;

       qDebug()<<"ImageInfo: "<<objImageDataPointer->GetHeight() <<endl;

       qDebug()<<"ImageInfo: "<<objImageDataPointer->GetPayloadSize() <<endl;

   }

};


#endif // CAMERAEVENT_H
