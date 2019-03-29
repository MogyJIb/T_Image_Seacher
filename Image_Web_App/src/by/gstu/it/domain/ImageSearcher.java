package by.gstu.it.domain;


public class ImageSearcher {
    private static final String SOLVER = "Image_Seacher_MPI.exe";

    private static final int PROCESS_COUNT = 5;
    public static int USE_MPI = 1;

    public void search() {
        ProcessHelper.runAndWait(buildCommands());
    }

    private String[] buildCommands() {
        String path = this.getClass().getClassLoader().getResource(SOLVER).getFile();
        if (path.startsWith("/") || path.startsWith("\\"))
            path = path.substring(1);
        path = path.substring(0, path.length() - SOLVER.length());

        return new String[]{ "mpiexec", "-n", String.valueOf(PROCESS_COUNT), path + SOLVER, path, String.valueOf(USE_MPI) };
    }

}
