import java.io.*;

public class LinkState {

	private static int[][] nodes = new int[51][51];
	private static int[] nodesTracker = new int[51];
	private static int[] hasBeenSelected;
	private static int noOfNodes;
	private static int sourceNode;
	private static int curNode;

	private static String input;

    public static void main(String args[])throws IOException {
    
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        input = in.readLine();
	
		//Initialize nodes array
		
		for(int i=0; i<51; i++) 
			for(int j=0; j<51; j++)
				nodes[i][j] = -1;
		for(int i=0; i<51; i++)
			nodes[i][i] = 0;

		//Parsing the String input
	    String[] lines = input.split(";");
		int linesNo = lines.length;
		for(int i=0; i<linesNo; i++) {
			lines[i] = lines[i].trim();
			String[] link = lines[i].split(", ");
			int val = Integer.parseInt(link[1]);
			String[] tmpNodes = link[0].split(":");
			int node1 = Integer.parseInt(tmpNodes[0]);
			int node2 = Integer.parseInt(tmpNodes[1]);
			nodes[node1][node2] = val;
			nodes[node2][node1] = val;
			nodesTracker[node1] = 1;
			nodesTracker[node2] = 1;
		}

		noOfNodes = findNoOfNodes();



		// nodes = new int[][]
		// 		{	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		// 			{0, 0, 4, 0, 0, 0, 0, 0, 8, 0}, 
  //                   {0, 4, 0, 8, 0, 0, 0, 0, 11, 0}, 
  //                   {0, 0, 8, 0, 7, 0, 4, 0, 0, 2}, 
  //                   {0, 0, 0, 7, 0, 9, 14, 0, 0, 0}, 
  //                   {0, 0, 0, 0, 9, 0, 10, 0, 0, 0}, 
  //                   {0, 0, 0, 4, 14, 10, 0, 2, 0, 0}, 
  //                   {0, 0, 0, 0, 0, 0, 2, 0, 1, 6}, 
  //                   {0, 8, 11, 0, 0, 0, 0, 1, 0, 7}, 
  //                   {0, 0, 0, 2, 0, 0, 0, 6, 7, 0} 
  //               };
  //       for(int i=1; i<10; i++) {
  //       	for(int j=1; j<10; j++) {
  //       		if(nodes[i][j] == 0 && i!=j)
  //       			nodes[i][j] = -1;
  //       	}
  //       }
  //       noOfNodes = 9;
  //       for (int i=1; i<=noOfNodes; i++) {
		// 	for(int j=1; j<=noOfNodes; j++) {
		// 		System.out.print(nodes[i][j] + " ");
		// 	}
		// 	System.out.println();
		// }
		

		hasBeenSelected = new int[noOfNodes+1];
		// System.out.println(noOfNodes);
		// printNodes();
		sourceNode = Integer.parseInt(in.readLine());
		hasBeenSelected[sourceNode] = 1;

		for(int i=1; i<=noOfNodes; i++){
			nodes[0][i] = nodes[sourceNode][i];
		}
		for(int i=1; i<=noOfNodes; i++){
			if(i == sourceNode)
				nodes[i][0] = 0;
			else if(nodes[0][i] == -1)
				nodes[i][0] = -1;
			else
				nodes[i][0] = i;
		}

		dijksitras();

		System.out.println("The final output is");
		printNodes();

		// for(int i=1; i<=noOfNodes; i++) {
		// 	System.out.println(i + " : " + nodes[sourceNode][i]);
		}	

	}


	public static void dijksitras() {

		while((curNode = minimumDistNode()) != 0) {
			hasBeenSelected[curNode] = 1;
			for(int i=1; i<=noOfNodes; i++) {
				if(i == sourceNode || !hasNotBeenSelected(i)) 
					continue;
				// nodes[sourceNode][i] = customMin(nodes[sourceNode][i], nodes[sourceNode][curNode], nodes[curNode][i]);
				int v, w, vw;
				v = nodes[sourceNode][i];
				w = nodes[sourceNode][curNode];
				vw = nodes[curNode][i];
				if(v == -1 && vw == -1){
					nodes[sourceNode][i] = -1;
				}
				else if (v == -1 && vw != -1){
					if(nodes[curNode][0] != 0)
						nodes[i][0] = nodes[curNode][0];
					// if(nodes[0][curNode] == -1)
					// 	nodes[i][0] = nodes[curNode][0];
					nodes[sourceNode][i] = w+vw;
				}
				else if (v != -1 && vw == -1){
					nodes[sourceNode][i] = v;
				}
				else if(v < w+vw){
					nodes[sourceNode][i] = v;
				} else {
					if(nodes[curNode][0] != 0)
						nodes[i][0] = nodes[curNode][0];
					// if(nodes[0][curNode] == -1)
					// 	nodes[i][0] = nodes[curNode][0];
					nodes[sourceNode][i] = w+vw;
				}
						
			}
			printNodes();
			// System.out.println("Seletected array");
			// for(int i=1; i<=noOfNodes; i++)
			// 	System.out.println(i + " " + hasBeenSelected[i]);
			// System.out.println();
		}


	}

	public static int customMin(int v, int w, int vw) {
		if(v == -1 && vw == -1)
			return -1;
		else if (v == -1 && vw != -1)
			return w+vw;
		else if (v != -1 && vw == -1)
			return v;
		else 
			return Math.min(v, w+vw);
	}


	public static int minimumDistNode() {
		int min = Integer.MAX_VALUE;
		int reqNode = 0;
		for(int i=1; i<=noOfNodes; i++) {
			if (i == sourceNode)
				continue;
			if((nodes[sourceNode][i] < min && nodes[sourceNode][i] != -1) && hasNotBeenSelected(i)){
				min = nodes[sourceNode][i];
				reqNode = i;
			}
		}
		return reqNode;
	}

	public static boolean hasNotBeenSelected(int i) {
		if(hasBeenSelected[i] == 0)
			return true;
		return false;
	}

	//To find the number of nodes
	public static int findNoOfNodes() {
		int len = 0;
		for (int i=0; i<51 ;i++ ) {
			len += nodesTracker[i];
		}
		return len;
	}

	public static void printNodes() {
		// for (int i=1; i<=noOfNodes; i++) {
		// 	for(int j=1; j<=noOfNodes; j++) {
		// 		System.out.print(nodes[i][j] + " ");
		// 	}
		// 	System.out.println();
		// }
		System.out.println("Current Node : "+ curNode);
		for(int i=1; i<=noOfNodes; i++) {
			// System.out.println(i + " : " + nodes[sourceNode][i]);
			System.out.println(i + " : " + nodes[i][0]);
		}
	}

}

