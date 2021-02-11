json.array!(@addresses_users) do |addresses_user|
  json.extract! addresses_user, :id, :user_id, :address_id
  json.url addresses_user_url(addresses_user, format: :json)
end
