package by.gstu.it.presentation;

import by.gstu.it.domain.ClassInjector;
import by.gstu.it.domain.ImageSearcher;
import com.google.gson.Gson;

import javax.imageio.ImageIO;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.Part;
import java.awt.image.BufferedImage;
import java.io.*;

public class ImageServlet extends HttpServlet {

    private final ImageSearcher searcher = ClassInjector.provideImageSearcher();

    private static final String UPLOAD_DIR = "WEB-INF" + File.separator + "classes";

    protected void doPost(HttpServletRequest request,
                          HttpServletResponse response) throws ServletException, IOException {


        readInputImages(request);
        searcher.search();
        sendResult(response);
    }

    private void sendResult(HttpServletResponse response) throws ServletException, IOException {
        response.setContentType("image/jpeg");

        try (BufferedInputStream bin = new BufferedInputStream(this.getClass().getClassLoader().getResourceAsStream("result.jpg"));
             BufferedOutputStream bout = new BufferedOutputStream(response.getOutputStream())) {
            int ch;
            while ((ch = bin.read()) != -1) bout.write(ch);
        }
    }

    private void readInputImages(HttpServletRequest request) throws ServletException, IOException {
        String uploadFilePath = request.getServletContext().getRealPath("") + File.separator + UPLOAD_DIR;
        File fileSaveDir = new File(uploadFilePath);
        if (!fileSaveDir.exists()) fileSaveDir.mkdirs();

        for (Part part : request.getParts())
            part.write(uploadFilePath + File.separator + getFileName(part));
    }

    private String getFileName(Part part) {
        String contentDisp = part.getHeader("content-disposition");
        System.out.println("content-disposition header= "+contentDisp);
        String[] tokens = contentDisp.split(";");
        String name = "";
        String p = "";
        for (String token : tokens) {
            String substring = token.substring(token.indexOf("=") + 2, token.length() - 1);
            if (token.trim().startsWith("name")) {
                name = substring;
            } else if (token.trim().startsWith("filename")) {
                p = substring.split("\\.")[1];
            }
        }
        return name + "." + p;
    }

}