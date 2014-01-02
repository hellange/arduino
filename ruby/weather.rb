require 'net/http'
require 'rexml/document'

# location Oppsal, Oslo, Norway
uri = URI.parse("http://api.met.no/weatherapi/locationforecast/1.8/?lat=59.8959;lon=10.8515")

response = Net::HTTP.get_response(uri);

xmlDoc = REXML::Document.new response.body
use_next = false;
xmlDoc.elements.each('weatherdata/product/time') do |element|

  from = element.attributes["from"]
  to = element.attributes["to"]

  if (use_next)
     value = -999
     max = - 999
     min = -999
     symbol_id = "EMPTY"
     element.elements.each("location/precipitation") do | t |
      value = t.attributes["value"];
      min = t.attributes["minvalue"];
      max = t.attributes["maxvalue"];
     end
     element.elements.each("location/symbol") do | t |
       symbol_id = t.attributes["id"];
     end
     puts "#{from} - #{to} : #{value} mm (#{min}-#{max}) #{symbol_id}"

  end

  
  if (to == from)
    element.elements.each("location/temperature") do | t |
      temperature = t.attributes["value"]
      unit = t.attributes["unit"]
      puts "#{from} - #{to} : #{temperature} C"
    end
    use_next = true
  else
    use_next = false
  end


end








