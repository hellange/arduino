require 'net/http'
require 'rexml/document'

uri = URI.parse("http://api.met.no/weatherapi/locationforecast/1.8/?lat=59.8959;lon=10.8515")
response = Net::HTTP.get_response(uri);

xmlDoc = REXML::Document.new response.body
use_next = false;
xmlDoc.elements.each('weatherdata/product/time') do |element|

  from = element.attributes["from"]
  to = element.attributes["to"]

  if (use_next)
     element.elements.each("location/precipitation") do | t |
      value = t.attributes["value"];
      min = t.attributes["minvalue"];
      max = t.attributes["maxvalue"];

      puts "#{from} - #{to} : #{value} mm (#{min}-#{max})"
     end
  end

  
  if (to == from)
     element.elements.each("location/temperature") do | t |
      puts "#{from} - #{to} : #{t.attributes["value"]} C"
     end
    use_next = true
  else
    use_next = false
  end


end








