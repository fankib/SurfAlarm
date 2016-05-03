package ch.beni.rss;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.util.UUID;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class RssServlet extends HttpServlet {

	private static final String REPLACE_DESCRIPTION = "%DESCRIPTION%";
	private static final String REPLACE_GUID = "%GUID%";
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	@Override
	protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

		BufferedReader templateReader = new BufferedReader(new InputStreamReader(getClass().getResourceAsStream("rsstemplate.xml")));
		StringBuilder sb = new StringBuilder();
		for ( String line; (line=templateReader.readLine()) != null; ){
			sb.append(line);
			sb.append("\n");
		}
		String template = sb.toString();
		
		// dirty:
		templateReader.close();

		StringBuilder sbDescription = new StringBuilder();

		try {
			Process p = Runtime.getRuntime().exec("surfalarm");
			p.waitFor();

			BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));

			for (String line; (line = reader.readLine()) != null;) {
				sbDescription.append(line);
				sbDescription.append("\n");
			}

		} catch (InterruptedException e) {
			throw new ServletException(e);
		}

		String guid = UUID.randomUUID().toString();
		
		// replace:
		String updateDescription = template.replace(REPLACE_DESCRIPTION, sbDescription.toString());
		String updateAll = updateDescription.replace(REPLACE_GUID, guid);
		
		resp.getWriter().write(updateAll);
		resp.getWriter().flush();
	}



}