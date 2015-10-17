package test.programs.arduino.communiate;
/**
 * This is a node for the arduino that keeps track
 * of informaton about arduinos. This will
 * also be used to condense and send information
 * @author Ian
 *
 */
public class ArduinoInfo
{
	private String deviceID = "000000";
	private String teamID = "DEFAULT";
	private int teamWidth = 0;
	private String teamColor = "WHITE";
	private float lon = 00;
	private float lat = 0;
	
	/**
	 * Sets the device Id. Returns false if the id
	 * given is not the correct format
	 * @param id
	 * @return
	 */
	public boolean setDeviceID(String id)
	{
		if(id.length() > 6)
		{
			return false;
		}
		else
		{
			deviceID = id;
			return true;
		}
		
	}
	public boolean setLon(float newLong)
	{
		lon = newLong;
		return true;
	}
	public boolean setLat(float newLat)
	{
		lat = newLat;
		return true;
	}
	public boolean setTeamID(String id)
	{
		teamID = id;
		return true;
	}
	
	public boolean setTeamWidth(int width)
	{
		teamWidth = width;
		return true;
	}
	
	public boolean SetColor(String color)
	{
		//use a for loop to iterate through the color list and make sure that we can
		//switch to that color
		teamColor = color;
		return true;
	}
	
	public String getDeviceID()
	{
		return deviceID;
	}
	
	public String getTeamID()
	{
		return teamID;
	}
	
	public int getTeamWidth()
	{
		return teamWidth;
	}
	
	public String getTeamColor()
	{
		return teamColor;
	}
	
	public float getLat()
	{
		return lat;
	}
	
	public float getLon()
	{
		return lon;
	}
}
