# TopazOnCMD
## 配置环境
读取同目录下的`TopazOnCMD.ini`以**配置环境**  
需要四行命令，两行为Topaz环境变量，另两行分别为Topaz所用ffmpeg和输出所用ffmpeg  
打开Topaz，Ctrl+T打开CMD，输入`set`，即可**获取Topaz环境变量**  
`TopazOnCMD.ini`内容示例:  
`TVAI_MODEL_DATA_DIR=C:\ProgramData\Topaz Labs LLC\Topaz Video AI\models`  
`TVAI_MODEL_DIR=C:\ProgramData\Topaz Labs LLC\Topaz Video AI\models`  
`TVAI_ffmpeg=D:\Topaz Video AI v3.3.2\App\Topaz Video AI\ffmpeg.exe`  
`Output_ffmpeg=D:\FFmpeg\ffmpeg-6.1-full_build\bin\ffmpeg.exe`  
**  **  
## 使用
打开Topaz，调整好要输出的视频，导出选项设置为 None音频 和 mkv  
Ctrl+Shift+E导出命令  
将命令输入到TopazOnCMD中  
*(**如果版本低于v1.0**，需要在命令末尾的输出路径前添加`^^^`，示例:`…前文略… "use_metadata_tags+write_colr" ^^^ "E:/视频/输出_tvai.mkv"`)*  
**  **  
## 原理
找到`"-c:v"`和`"-an"`，删除二者之间（不包括`"-an"`）的字符  
将字符头部的`ffmpeg`替换为`%TVAI_ffmpeg%`  
在第二个路径前添加`ffmpeg管道 -pix_fmt yuv422p yuv4mpegpipe -> %Output_ffmpeg% + 编码器参数`  
**  **  
第一次用C++写程序，也是第一次用Git，所以整个项目都是一堆问题，就当瞎写着玩的（
