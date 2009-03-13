
import vtk.*;

import java.awt.BorderLayout;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Observable;
import java.util.Observer;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

public class MiniViz extends vtkCanvas {

  private final static int width = 700;
  private final static int height = 700;

  static { 
    System.loadLibrary("vtkRealityGridSteeringJava");
  }

  private vtkGenericRenderWindowInteractor interactor;
  private vtkPolyData surface;
  private vtkTextActor text;

  private vtkRealityGridDataReader dataReader;
  private vtkRealityGridIOChannel ioChannel;

  private int num_points_old = -1;

  public MiniViz() {
    super();

    // Certain things cannot be done until we actually
    // have a window on the screen. Attach observer to
    // set the render window size and start the polling
    // loop after the render window is created.
    addWindowSetObserver(new Observer() {
        public void update(Observable o, Object arg) {
          setSize(width, height);
	  dataReader.SetInteractor(interactor);
        }
      });

    createVTKPipeline();

    SwingUtilities.invokeLater(new Runnable() {
	public void run() {	  
	  createAndShowGUI();
	}
      });
  }

  private void createAndShowGUI() {
    JPanel p = new JPanel();
    p.setLayout(new BorderLayout());
    p.add(this, BorderLayout.CENTER);

    JFrame frame = new JFrame("RealityGrid Java Data Reader Test");
    frame.setBounds(10, 10, width, height);
    frame.getContentPane().add(p, BorderLayout.CENTER);
    frame.setVisible(true);
    frame.pack();

    frame.addWindowListener(new WindowAdapter() {
	public void windowClosing(WindowEvent e) {
	  System.exit(0);
	}
      });
  }

  private void createVTKPipeline() {
    surface = new vtkPolyData();

    vtkPolyDataNormals normals = new vtkPolyDataNormals();
    normals.SetInput(surface);
    normals.ConsistencyOn();
    normals.SplittingOff();

    vtkPolyDataMapper surface_map = new vtkPolyDataMapper();
    surface_map.SetInputConnection(normals.GetOutputPort());

    vtkActor surface_actor = new vtkActor();
    surface_actor.SetMapper(surface_map);

    // create frame text label
    text = new vtkTextActor();
    text.SetInput("Data size: 000000");
    text.GetPositionCoordinate().SetCoordinateSystemToNormalizedViewport();
    text.GetPositionCoordinate().SetValue(0.70, 0.95);
    text.GetTextProperty().SetFontSize(24);
    text.GetTextProperty().SetFontFamilyToArial();
    text.GetTextProperty().BoldOn();
    text.GetTextProperty().ItalicOn();

    vtkRenderer renderer = this.GetRenderer();
    renderer.AddActor(surface_actor);
    renderer.AddActor(text);
    ren.SetBackground(0.1, 0.2, 0.4);

    interactor = this.getIren();
    vtkInteractorStyleSwitch interactorStyle = new vtkInteractorStyleSwitch();
    interactorStyle.SetCurrentStyleToTrackballCamera();
    interactor.SetInteractorStyle(interactorStyle);

    // create the io channel and register redrawing callback
    ioChannel = new vtkRealityGridIOChannel();
    ioChannel.SetName("mini_app data in");
    ioChannel.SetIODirection(0);
    ioChannel.AddObserver("UserEvent", this, "redrawCallback");

    // create the reader and register the io channel with it
    dataReader = new vtkRealityGridDataReader();
    dataReader.RegisterIOChannel(ioChannel, 10);
  }

  public void redrawCallback() {
    int side_length;
    int num_points;

    vtkRealityGridDataSliceCollection slices = ioChannel.GetDataSlicesIn();

    vtkRealityGridIntDataSlice slice =
      (vtkRealityGridIntDataSlice) slices.GetDataSlice(0);

    side_length = slice.GetData().GetJavaArray()[0];
    num_points = side_length * side_length;

    vtkRealityGridFloatDataSlice points_slice =
      (vtkRealityGridFloatDataSlice) slices.GetDataSlice(1);

    float[] data = points_slice.GetData().GetJavaArray();

    // only rebuild surface if number of points has changed
    if(num_points != num_points_old) {
      // create points
      vtkPoints points = new vtkPoints();
      points.SetNumberOfPoints(num_points);
      int index = 0;
      for(int j = 0; j < side_length; j++) {
	for(int i = 0; i < side_length; i++) {
	  points.SetPoint(index, i, j, data[index]);
	  index++;
	}
      }

      // create triangle strips
      vtkCellArray strips = new vtkCellArray();
      for(int i = 1; i < side_length; i++) {
	strips.InsertNextCell(side_length * 2);
	for(int j = 0; j < side_length; j++) {
	  strips.InsertCellPoint((side_length * j) + (i - 1));
	  strips.InsertCellPoint((side_length * j) + i);
	}
      }

      surface.SetPoints(points);
      surface.SetStrips(strips);

      text.SetInput(String.format("Data size: %6d", num_points));

      num_points_old = num_points;
    }
    else {
      // adjust point positions
      vtkPoints points = surface.GetPoints();
      int index = 0;
      for(int j = 0; j < side_length; j++) {
	for(int i = 0; i < side_length; i++) {
	  points.SetPoint(index, i, j, data[index]);
	  index++;
	}
      }

      points.Modified();
    }
  }

  public static void main(String[] args) {
    new MiniViz();
  }
}
