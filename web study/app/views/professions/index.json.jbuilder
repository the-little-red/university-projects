json.array!(@professions) do |profession|
  json.extract! profession, :id, :name, :salary, :user_id
  json.url profession_url(profession, format: :json)
end
