json.array!(@emails) do |email|
  json.extract! email, :id, :email_address, :user_id
  json.url email_url(email, format: :json)
end
