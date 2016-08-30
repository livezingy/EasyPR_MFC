# EasyPR_MFC
1. EasyPR是一个开源的中文车牌识别系统，其目标是成为一个简单，高效，准确的车牌识别库。该开源系统基于openCV开源库，可以移植到opencv支持的所有平台，在Github上有全部的源代码https://github.com/liuruoze/EasyPR。该开源系统可以作为学习openCV的一个极好的范例。 
2. 作者不仅技术实力深厚，写作水平也是一流。博客中有EasyPR开发过程以及相关原理的详细讲解，且同时整理了一份PDF格式EasyPR的开发详解，在这里对作者的辛苦付出与无私开源致以崇高的敬意。EasyPR作者的博客：http://www.cnblogs.com/subconscious/
3. 车牌识别系统easyPR自带的demo基于win32控制台，识别图片是在程序中指定的，必须通过修改程序来修改待识别图片。为了更好的学习这个系统和openCV，我尝试建立了一个MFC的可视化界面。初步规划的easyPR的MFC可视化界面功能如下：用户可以指定当前需要识别的图片，在界面中可以观察到源图像，识别结果，可以选择观察车牌识别的中间过程，中间过程可以显示在源图片的左上角，例如车牌定位，车牌判断，字符分割等等。关于EasyPR的MFC可视化界面的更多信息可以访问http://www.livezingy.com
