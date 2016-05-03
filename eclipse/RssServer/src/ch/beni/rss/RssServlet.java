package ch.beni.rss;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.UUID;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class RssServlet extends HttpServlet {

	private static final String REPLACE_DESCRIPTION = "%DESCRIPTION%";
	private static final String REPLACE_GUID = "%GUID%";
	private static final String REPLACE_PUBDATE = "%PUBDATE%";

	private static final long FIVE_MINUTES = 1000 * 60 * 5;

	private static final SimpleDateFormat sdf = new SimpleDateFormat("dd.MM.yyyy HH:mm");

	private String response = null;
	private long latestTimestamp = 0;

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	@Override
	protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

		long now = new Date().getTime();

		if (response == null || now - latestTimestamp > FIVE_MINUTES) {

			String description;
			try {
				Process p = Runtime.getRuntime().exec("./surfalarm");
				// Process p = Runtime.getRuntime().exec("pwd");
				p.waitFor();

				BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));

				StringBuilder sbDescription = new StringBuilder();
				for (String line; (line = reader.readLine()) != null;) {
					sbDescription.append(line);
					sbDescription.append("\n");
				}
				description = sbDescription.toString();

			} catch (InterruptedException e) {
				throw new ServletException(e);
			}

			if (response == null || description.contains("Maybe") || description.contains("Yes")) {
				// update latestValue
				updateResponse(description, now);
			}
		}

		resp.getWriter().write(response);
		resp.getWriter().flush();

	}

	private void updateResponse(String description, long now) throws IOException {
		String guid = UUID.randomUUID().toString();
		String pubDate = sdf.format(new Date());

		BufferedReader templateReader = new BufferedReader(
				new InputStreamReader(getClass().getResourceAsStream("rsstemplate.xml")));
		StringBuilder sb = new StringBuilder();
		for (String line; (line = templateReader.readLine()) != null;) {
			sb.append(line);
			sb.append("\n");
		}
		String template = sb.toString();
		// dirty:
		templateReader.close();

		// replace:
		String updated = template.replace(REPLACE_DESCRIPTION, description) //
				.replace(REPLACE_GUID, guid) //
				.replace(REPLACE_PUBDATE, pubDate);

		response = updated;
		latestTimestamp = now;
	}

}