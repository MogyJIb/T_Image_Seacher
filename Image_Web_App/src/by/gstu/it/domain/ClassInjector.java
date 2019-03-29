package by.gstu.it.domain;

public class ClassInjector {

    private static ImageSearcher imageSearcher;

    public static ImageSearcher provideImageSearcher() {
        if (imageSearcher == null) imageSearcher = new ImageSearcher();
        return imageSearcher;
    }

}
