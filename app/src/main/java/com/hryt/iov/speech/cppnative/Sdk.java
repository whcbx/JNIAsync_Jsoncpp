package com.hryt.iov.speech.cppnative;

/**
 * Created by jiong103 on 2017/3/23.
 */

public class Sdk {
    private Sdk() {
    }

    //单例
    private static class SdkHodler {
        static Sdk instance = new Sdk();
    }

    public static Sdk getInstance() {
        return SdkHodler.instance;
    }

    //调到C层的方法
    public native void nativeDownload();

    //c层回调上来的方法
    private int onProgressCallBack(String total, int already) {
        //自行执行回调后的操作
        System.out.println("total:"+total);
        System.out.println("already:"+already);
        return 1;
    }
}