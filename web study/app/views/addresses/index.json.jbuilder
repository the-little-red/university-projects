json.array!(@addresses) do |address|
  json.extract! address, :id, :address_house, :city, :country
  json.url address_url(address, format: :json)
end
